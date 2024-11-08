#ifndef INTERVALOMETER_H
#define INTERVALOMETER_H

#include "hardwaretimer.h"
#include "config.h"
#include <EEPROM.h>
#include "axis.h"

#ifdef STEPPER_0_9
#define ARCSEC_PER_STEP 2.0
#else
#define ARCSEC_PER_STEP 4.0
#endif

extern void sendError(String errorMessage);


class Intervalometer {

  enum State { INACTIVE,
               PRE_DELAY,
               CAPTURE,
               DITHER,
               PAN,
               DELAY,
               REWIND };

  enum Mode { LONG_EXPOSURE_STILL,
              LONG_EXPOSURE_MOVIE,
              DAY_TIME_LAPSE,
              DAY_TIME_LAPSE_PAN };

  struct Settings {           //20 bytes
    Mode mode;
    uint16_t exposures;       //2b
    uint16_t delay_time;      //seconds, max limt 18 h, 12 mins //2b
    uint16_t pre_delay_time;  //seconds //2b
    uint16_t exposure_time;   //seconds, max limt 18 h, 12 mins //2b
    float pan_angle;          //degrees //4b
    uint8_t panDirection; //
    bool dither;              //1b
    uint8_t dither_frequency; //
    bool post_tracking_on;    //1b
    uint16_t frames;          //2b
    float pixel_size;         //micrometre (um) //4b
    uint16_t focal_length;    //mm //2b
  };


public:
  State currentState;
  Settings currentSettings;
  Settings presets[5];
  bool nextState;
  bool intervalometerActive;
  Intervalometer(uint8_t triggerPinArg);
  void saveSettingsToPreset(uint8_t preset);
  void readSettingsFromPreset(uint8_t preset);
  void run();

private:
  HardwareTimer intervalometerTimer;
  bool timerStarted;
  void savePresetsToEEPPROM();
  void readPresetsFromEEPROM();
  uint8_t triggerPin;
  uint16_t exposures_taken;
  uint16_t frames_taken;
  uint16_t getStepsPerTenPixels();
  float getArcsec_per_pixel();
  template<class T> int writeObjectToEEPROM(int address, const T& object);
  template<class T> int readObjectFromEEPROM(int address, T& object);
  void runDither();
};

extern Intervalometer intervalometer;

#endif