#include "tmc_motor_driver.h"
#include "uart.h"

TmcMotorDriver::TmcMotorDriver(HardwareSerial* serialPort, uint8_t addr, float rSense,
                               uint8_t rxPin, uint8_t txPin)
    : serialPort(serialPort), addr(addr), tmc_driver(serialPort, rSense, addr)
{
    if (serialPort != nullptr)
    {
        serialPort->begin(115200, SERIAL_8N1, rxPin, txPin);
        while (!serialPort)
            ;
    }

    tmc_driver.begin();
    tmc_driver.pdn_disable(true);      // enable pdn_uart
    tmc_driver.mstep_reg_select(true); // Use MRES register for microstepping
    tmc_driver.push();                 // Push settings to driver
}

void TmcMotorDriver::setMicrosteps(uint16_t microsteps)
{
    tmc_driver.microsteps(microsteps);
    tmc_driver.rms_current(700, 1.2); // Set RMS current to 700 mA, hold current to 840 mA
    tmc_driver.iholddelay(10);        // Set hold delay
    tmc_driver.push();

    uint16_t actual_microsteps = tmc_driver.microsteps();
    uint16_t actual_rms_current = tmc_driver.rms_current();

    print_out("Microsteps changed:");
    print_out("  Requested: %d, Actual: %d", microsteps, actual_microsteps);
    print_out("  RMS Current: %d mA", actual_rms_current);

    if (microsteps != actual_microsteps)
    {
        print_out("WARNING: Microstep setting mismatch!");
    }
}

void TmcMotorDriver::setDirection(bool direction)
{
    tmc_driver.shaft(direction);
    tmc_driver.push();
}
