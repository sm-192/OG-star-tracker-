#ifndef AXIS_H
#define AXIS_H

#include "timer.h"


class Axis {

  Timer stepTimer;
  Timer slewTimeOutTimer;


  void IRAM_ATTR timer_web_timeout_ISR() {
    handleSlewOff();
  }

  void IRAM_ATTR timer_tracking_ISR() {
    //tracking ISR
    digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));  //toggle step pin at required frequency
  }
}

#endif