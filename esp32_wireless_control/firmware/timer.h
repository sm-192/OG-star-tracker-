#ifndef TIMER_H
#define TIMER_H



class Timer {

  hw_timer_t* timer_pointer = NULL;

  public:

  void (*ISR_Function)(void);

  Timer(uint8_t timerNumber, uint16_t divider, void (*functionToCall)());


  void start(uint64_t alarmValue, bool autoReload);
  void stop();
  void setCountValue(uint64_t countValue);
};


#endif