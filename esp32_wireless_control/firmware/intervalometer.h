#ifndef INTERVALOMETER_H
#define INTERVALOMETER_H

#include "hardwaretimer.h"
#include "config.h"
#include "eepromhandler.h"



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

  struct Settings { //20 bytes
    uint16_t exposures;      //2b
    uint16_t delay_time; //seconds, max limt 18 h, 12 mins //2b
    uint16_t pre_delay_time; //seconds //2b
    float pan_angle; //degrees //4b
    bool dither; //1b
    bool post_tracking_on; //1b
    uint16_t frames; //2b
    float pixel_size; //micrometre (um) //4b
    uint16_t focal_length; //mm //2b
  };


public:
  State currentState;
  Mode currentMode;
  Settings currentSettings; 
  Settings presets[5];
  bool nextState;
  bool intervalometerActive;
  Intervalometer(uint8_t triggerPin);
  void saveSettingsToPreset(uint8_t preset);
  void readSettingsFromPresets(uint8_t preset);

private:
  HardwareTimer intervalometerTimer;
  void savePresetsToEEPPROM();
  void readPresetsFromEEPROM();
  uint16_t exposures_taken;
  uint16_t frames_taken;
  uint16_t getStepsPerTenPixels(); 
  float getArcsec_per_pixel();
};

#endif