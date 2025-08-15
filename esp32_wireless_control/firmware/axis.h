#ifndef AXIS_H
#define AXIS_H

#include "configs/config.h"
#include "configs/consts.h"
#include "hardwaretimer.h"
#include "motor_driver.h"

#include "tracking_rates.h"

extern HardwareTimer slewTimeOut;

class Position
{
  public:
    int64_t arcseconds;

    Position(int degrees = 0, int minutes = 0, float seconds = 0.0f);
    float toDegrees() const;
    static int64_t toArcseconds(int degrees, int minutes, float seconds);
};

class Direction
{
  public:
    bool tracking;
    bool requested;
    volatile bool absolute;
};

class Rate
{
  public:
    uint64_t tracking;
    uint64_t requested;
};

class Axis
{
  public:
    Axis(uint8_t axisNumber, MotorDriver* driver, uint8_t dirPinforAxis, bool invertDirPin);

    void setAxisTargetCount(int64_t count);
    int64_t getAxisTargetCount();
    void resetAxisCount();
    void setAxisCount(int64_t count);
    int64_t getAxisCount();

    void startTracking(uint64_t rate, bool directionArg);
    void stopTracking();
    void startSlew(uint64_t rate, bool directionArg);
    void stopSlew();

    void gotoTarget(uint64_t rate, const Position& current, const Position& target);
    void stopGotoTarget();

    volatile int64_t axisCountValue;
    volatile int64_t targetCount;
    volatile bool goToTarget;
    bool slewActive;
    bool trackingActive;

    Direction direction;

    volatile bool counterActive;

    Rate rate;

    uint16_t getMicrostep()
    {
        return microStep;
    }

    volatile int64_t position;
    void resetPosition()
    {
        setPosition(0);
    }
    void setPosition(int64_t pos)
    {
        position = pos;
    }
    int64_t getPosition()
    {
        return position;
    }

    void requestTracking(uint64_t requestedRate, bool requestedDirection)
    {
        rate.requested = requestedRate;
        direction.requested = requestedDirection;
        startRequested = true;
    }

    bool trackingRequested()
    {
        return startRequested;
    }

    void begin();

    void print_status();

  private:
    void setDirection(bool directionArg);
    void setMicrostep(uint16_t microstep);

    HardwareTimer stepTimer;
    uint16_t microStep;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t axisNumber;
    bool invertDirectionPin;
    MotorDriver* driver;
    volatile bool startRequested;
};

extern Axis ra_axis;

#endif
