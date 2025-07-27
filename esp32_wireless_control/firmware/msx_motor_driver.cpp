#include <Arduino.h>

#include "msx_motor_driver.h"
#include "uart.h"

MSxMotorDriver::MSxMotorDriver(uint8_t ms1Pin, uint8_t ms2Pin, uint8_t dirPin)
    : _ms1Pin(ms1Pin), _ms2Pin(ms2Pin), _dirPin(dirPin)
{
    pinMode(_ms1Pin, OUTPUT);
    pinMode(_ms2Pin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
}

void MSxMotorDriver::setMicrosteps(uint16_t microsteps)
{
    switch (microsteps)
    {
        case 8:
            digitalWrite(_ms1Pin, LOW);
            digitalWrite(_ms2Pin, LOW);
            break;
        case 16:
            digitalWrite(_ms1Pin, HIGH);
            digitalWrite(_ms2Pin, HIGH);
            break;
        case 32:
            digitalWrite(_ms1Pin, HIGH);
            digitalWrite(_ms2Pin, LOW);
            break;
        case 64:
            digitalWrite(_ms1Pin, LOW);
            digitalWrite(_ms2Pin, HIGH);
            break;
        default:
            print_out("Invalid microstep value: %d", microsteps);
            break;
    }
}

void MSxMotorDriver::setDirection(bool direction)
{
    digitalWrite(_dirPin, direction);
}
