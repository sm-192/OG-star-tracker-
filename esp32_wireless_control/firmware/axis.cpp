#include "soc/gpio_struct.h"

#include "axis.h"
#include "uart.h"

#if MICROSTEPPING_MOTOR_DRIVER == USE_MSx_PINS_MICROSTEPPING
#include "msx_motor_driver.h"
MSxMotorDriver ra_driver(RA_MS1, RA_MS2, AXIS1_DIR);
#elif MICROSTEPPING_MOTOR_DRIVER == USE_TMC_DRIVER_MICROSTEPPING
#include "tmc_motor_driver.h"
TmcMotorDriver ra_driver(&AXIS_SERIAL_PORT, AXIS1_ADDR, TMC_R_SENSE, AXIS_RX, AXIS_TX);
#else
#error Unknown Motor Driver
#endif

Axis ra_axis(1, &ra_driver, AXIS1_DIR, RA_INVERT_DIR_PIN);

volatile bool ra_axis_step_phase = 0;

void IRAM_ATTR stepTimerRA_ISR()
{
    // ra ISR
    ra_axis_step_phase = !ra_axis_step_phase;
    if (ra_axis_step_phase)
    {
#ifdef BOARD_HAS_PIN_REMAP
        digitalWrite(AXIS1_STEP, HIGH);
#else
        GPIO.out_w1ts = (1 << AXIS1_STEP); // Set pin high
#endif
    }
    else
    {
#ifdef BOARD_HAS_PIN_REMAP
        digitalWrite(AXIS1_STEP, LOW);
#else
        GPIO.out_w1tc = (1 << AXIS1_STEP); // Set pin low
#endif
    }

    int64_t position = ra_axis.getPosition();
    uint16_t uStep = ra_axis.getMicrostep();
    if (ra_axis_step_phase)
    {
        if (ra_axis.direction.absolute ^ ra_axis.direction.tracking)
        {
            position -= MAX_MICROSTEPS / (uStep ? uStep : 1);
        }
        else
        {
            position += MAX_MICROSTEPS / (uStep ? uStep : 1);
        }
        ra_axis.setPosition(position);
    }

    if (ra_axis.counterActive && ra_axis_step_phase)
    { // if counter active
        int temp = ra_axis.getAxisCount();
        if (ra_axis.direction.absolute ^ ra_axis.direction.tracking)
        {
            temp--;
        }
        else
        {
            temp++;
        }
        ra_axis.setAxisCount(temp);
        if (ra_axis.goToTarget && ra_axis.getAxisCount() == ra_axis.getAxisTargetCount())
        {
            print_out("GotoTarget reached");
            print_out("GotoTarget axisCountValue: %lld", ra_axis.getAxisCount());
            print_out("GotoTarget targetCount: %lld", ra_axis.getAxisTargetCount());
            ra_axis.goToTarget = false;
            ra_axis.stopSlew();
        }
    }
}

void IRAM_ATTR slewTimeOutTimer_ISR()
{
    ra_axis.stopSlew();
}

HardwareTimer slewTimeOut(2000, &slewTimeOutTimer_ISR);

// Position class implementation
Position::Position(int degrees, int minutes, float seconds)
{
    arcseconds = toArcseconds(degrees, minutes, seconds);
}

float Position::toDegrees() const
{
    return arcseconds / 3600.0f;
}

int64_t Position::toArcseconds(int degrees, int minutes, float seconds)
{
    return (degrees * 3600) + (minutes * 60) + static_cast<int>(seconds);
}

void axisTask(void* parameter)
{
    Axis* axis = (Axis*) parameter;
    for (;;)
    {
        if (axis->trackingRequested())
        {
            axis->startTracking(axis->rate.requested, axis->direction.requested);
        }
        vTaskDelay(1);
    }
}

Axis::Axis(uint8_t axis, MotorDriver* motorDriver, uint8_t dirPinforAxis, bool invertDirPin)
    : stepTimer(TIMER_APB_CLK_FREQ), startRequested(false)
{
    driver = motorDriver;
    axisNumber = axis;
    direction.tracking = c_DIRECTION;
    dirPin = dirPinforAxis;
    invertDirectionPin = invertDirPin;
    rate.tracking = trackingRates.getRate();

    pinMode(dirPin, OUTPUT);

    switch (axisNumber)
    {
        case 1:
            stepTimer.attachInterupt(&stepTimerRA_ISR);
            break;
    }
}

void Axis::begin()
{
    if (xTaskCreatePinnedToCore(axisTask, "axis_task", 4096, this, 1, NULL, 1))
        print_out_nonl("Started axis task\n");
}

void Axis::startTracking(uint64_t rateArg, bool directionArg)
{
    startRequested = false;
    rate.tracking = rateArg;
    direction.tracking = directionArg;
    setDirection(directionArg);
    trackingActive = true;
    stepTimer.stop();
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING);
    stepTimer.start(rate.tracking, true);
}

void Axis::stopTracking()
{
    trackingActive = false;
    stepTimer.stop();
}

void Axis::gotoTarget(uint64_t rateArg, const Position& current, const Position& target)
{
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING / 2);
    int64_t deltaArcseconds = -1 * (target.arcseconds - current.arcseconds);
    int64_t stepsPerSecond = trackingRates.getStepsPerSecondSolar();

    print_out_nonl("deltaArcseconds: %lld\n", deltaArcseconds);

    if (abs(deltaArcseconds) > 86400 / 2)
    {
        if (deltaArcseconds > 0)
        {
            deltaArcseconds = (deltaArcseconds - 86400) % 86400;
        }
        else
        {
            deltaArcseconds = (deltaArcseconds + 86400) % 86400;
        }
    }

    int64_t stepsToMove =
        (deltaArcseconds * stepsPerSecond) / (MAX_MICROSTEPS / (microStep ? microStep : 1));
    bool directionTmp = (stepsToMove < 0) ^ direction.tracking;

    print_out_nonl("stepsToMove: %lld\n", stepsToMove);

    setPosition(current.arcseconds * stepsPerSecond);
    resetAxisCount();
    setAxisTargetCount(stepsToMove);

    if (targetCount != axisCountValue)
    {
        counterActive = true;
        goToTarget = true;
        stepTimer.stop();
        setDirection(directionTmp);
        slewActive = true;
        stepTimer.start(rateArg, true);
    }
}

void Axis::stopGotoTarget()
{
    goToTarget = false;
    counterActive = false;
    stepTimer.stop();
    slewTimeOut.start(1, true);
}

void Axis::startSlew(uint64_t rate, bool directionArg)
{
    stepTimer.stop();
    setDirection(directionArg);
    slewActive = true;
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING / 2);
    slewTimeOut.start(12000, true);
    stepTimer.start(rate, true);
}

void Axis::stopSlew()
{
    slewActive = false;
    stepTimer.stop();
    slewTimeOut.stop();
    if (trackingActive)
    {
        requestTracking(rate.tracking, direction.tracking);
    }
}

void Axis::setAxisTargetCount(int64_t count)
{
    targetCount = count;
}

int64_t Axis::getAxisTargetCount()
{
    return targetCount;
}

void Axis::resetAxisCount()
{
    axisCountValue = 0;
}

void Axis::setAxisCount(int64_t count)
{
    axisCountValue = count;
}

int64_t Axis::getAxisCount()
{
    return axisCountValue;
}

void Axis::setDirection(bool directionArg)
{
    direction.absolute = directionArg;
    driver->setDirection(directionArg ^ invertDirectionPin);
}

void Axis::setMicrostep(uint16_t microstep)
{
    if (microStep != microstep)
    {
        microStep = microstep;
        driver->setMicrosteps(microstep);
    }
}

void Axis::print_status()
{
    driver->print_status();
}
