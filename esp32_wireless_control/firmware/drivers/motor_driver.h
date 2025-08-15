#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_ 1

#include <stdint.h>

class MotorDriver
{
  public:
    MotorDriver(){};
    virtual void setMicrosteps(uint16_t microsteps) = 0;
    virtual void print_status() = 0;
    virtual void setDirection(bool direction) = 0;
};

#endif /* _MOTOR_DRIVER_H_ */
