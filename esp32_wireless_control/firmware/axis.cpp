#include "axis.h"

Axis ra_axis(1, true, AXIS1_DIR, RA_INVERT_DIR_PIN);
Axis dec_axis(2, false, AXIS2_DIR, DEC_INVERT_DIR_PIN);

volatile bool ra_axis_step_phase = 0;
volatile bool dec_axis_step_phase = 0;

void IRAM_ATTR stepTimerRA_ISR() {
  //tracking ISR
  ra_axis_step_phase = !ra_axis_step_phase;
  digitalWrite(AXIS1_STEP, ra_axis_step_phase);       //toggle step pin at required frequency
  if (ra_axis.counterActive && ra_axis_step_phase) {  //if counter active
    ra_axis.axisAbsoluteDirection ? ra_axis.axisCountValue++ : ra_axis.axisCountValue--;
    if (ra_axis.goToTarget && ra_axis.axisCountValue >= ra_axis.targetCount) {
      ra_axis.goToTarget = false;
      ra_axis.stopSlew();
    }
  }
}

void IRAM_ATTR stepTimerDEC_ISR() {
  //tracking ISR
  dec_axis_step_phase = !dec_axis_step_phase;
  digitalWrite(AXIS2_STEP, dec_axis_step_phase);                                         //toggle step pin at required frequency
  if (dec_axis_step_phase && dec_axis.counterActive) {                                   //if counter active
    dec_axis.axisAbsoluteDirection ? dec_axis.axisCountValue++ : dec_axis.axisCountValue--;  //TO_FIX
  }
}

void IRAM_ATTR slewTimeOutTimer_ISR() {
  //handleSlewOff();
}

HardwareTimer slewTimeOut(2000, &slewTimeOutTimer_ISR);


Axis::Axis(uint8_t axis, bool defaultTrackingOn, uint8_t dirPinforAxis, bool invertDirPin)
  : stepTimer(40000000) {
  axisNumber = axis;
  trackingDirection = c_DIRECTION;
  dirPin = dirPinforAxis;
  invertDirectionPin = invertDirPin;
  trackingRate = TRACKING_SIDEREAL;
  switch (axisNumber) {
    case 1:
      stepTimer.attachInterupt(&stepTimerRA_ISR);
      break;
    case 2:
      stepTimer.attachInterupt(&stepTimerDEC_ISR);
      break;
  }

  if (defaultTrackingOn && axisNumber == 1) {
    startTracking(TRACKING_SIDEREAL, trackingDirection);
  }
}


void Axis::startTracking(trackingRateS rate, bool directionArg) {
  trackingRate = rate;
  trackingDirection = axisAbsoluteDirection = directionArg;
  setDirection(axisAbsoluteDirection);
  trackingActive = true;
  stepTimer.stop();
  setMicrostep(16);
  stepTimer.start(trackingRate, true);
}

void Axis::stopTracking() {
  trackingActive = false;
  stepTimer.stop();
}

void Axis::startSlew(uint64_t rate, bool directionArg) {
  stepTimer.stop();
  axisAbsoluteDirection = directionArg;
  setDirection(axisAbsoluteDirection);
  slewActive = true;
  setMicrostep(8);
  slewTimeOut.start(12000, true);
  stepTimer.start(rate, true);
}

void Axis::stopSlew() {
  slewActive = false;
  stepTimer.stop();
  slewTimeOut.stop();
  if (trackingActive) {
    startTracking(trackingRate, trackingDirection);
  }
}

void Axis::setAxisTargetCount(int64_t count) {
  targetCount = count;
}

void Axis::resetAxisCount() {
  axisCountValue = 0;
}

void Axis::setDirection(bool directionArg) {
  digitalWrite(dirPin, directionArg ^ invertDirectionPin);
}

void Axis::setMicrostep(uint8_t microstep) {
  switch (microstep) {
    case 8:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, LOW);
      break;
    case 16:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, HIGH);
      break;
    case 32:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      break;
    case 64:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);
      break;
  }
}
