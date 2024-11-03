#ifndef AXIS_H
#define AXIS_H

#include "hardwaretimer.h"
#include "config.h"
#include <memory>

extern HardwareTimer slewTimeOut;

#ifdef STEPPER_0_9
enum TRACKING_RATES { TRACKING_SIDEREAL = 2659383,  //SIDEREAL (23h,56 min)
                      TRACKING_SOLAR = 2666666,     //SOLAR (24h)
                      TRACKING_LUNAR = 2723867 };   //LUNAR (24h, 31 min)
#else                                               //stepper 1.8 deg
enum TRACKING_RATES { TRACKING_SIDEREAL = 5318765,  //SIDEREAL (23h,56 min)
                      TRACKING_SOLAR = 5333333,     //SOLAR (24h)
                      TRACKING_LUNAR = 5447735 };   //LUNAR (24h, 31 min)
#endif


class Axis {


public:

  Axis(int axisNumber, bool defaultTrackingOn, int dirPinforAxis, bool invertDirPin);
  void startTracking(TRACKING_RATES rate, bool directionArg);
  void stopTracking();
  void startSlew(uint64_t rate, bool directionArg);
  void stopSlew();
  void setDirection(bool directionArg);
  static void setMicrostep(int microstep);
  
  volatile int64_t axis_counter;
  bool slewActive;
  bool trackingActive;
  volatile bool trackingDirection;
  bool slewDirection;
  bool invertDirectionPin;
  volatile bool counterActive;
  int axisNumber;

  TRACKING_RATES tracking_rate;

private:
  HardwareTimer stepTimer;
  int stepPin;
  int dirPin;
  static const int MS1Pin = MS1;
  static const int MS2Pin = MS2;
};

extern Axis ra_axis;
// extern Axis dec_axis;

#endif