#include "esp32-hal-timer.h"
#include "timer.h"


Timer::Timer(uint8_t timerNumber, uint16_t divider, void (*functionToCall)()) {
  this->ISR_Function = functionToCall;
  switch (timerNumber) {
    case 0:
      timer_pointer = timerBegin(0, divider, true);
      break;
    case 1:
      timer_pointer = timerBegin(1, divider, true);
      break;
    case 2:
      timer_pointer = timerBegin(2, divider, true);
      break;
    case 3:
      timer_pointer = timerBegin(3, divider, true);
      break;
  }
  timerAttachInterrupt(timer_pointer, ISR_Function, true);
}

void Timer::start(uint64_t alarmValue, bool autoReload) {
  timerAlarmWrite(timer_pointer, alarmValue, autoReload);
  timerAlarmEnable(timer_pointer);
}

void Timer::stop() {
  timerAlarmDisable(timer_pointer);
}

void Timer::setCountValue(uint64_t countValue) {
  timerWrite(timer_pointer, countValue);
}


