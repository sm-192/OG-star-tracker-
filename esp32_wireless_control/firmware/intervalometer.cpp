#include "intervalometer.h"

Intervalometer intervalometer(INTERV_PIN);

void intervalometer_ISA() {
  intervalometer.nextState = true;
}



Intervalometer::Intervalometer(uint8_t triggerPinArg)
  : intervalometerTimer(2000, intervalometer_ISA) {  //2kHz resolution of 0.5 ms
  EEPROM.begin(512);                                 //SIZE = 5 x presets = 5 x 32 bytes = 160 bytes
  currentState = INACTIVE;
  nextState = false;
  intervalometerActive = false;
  readPresetsFromEEPROM();
  timerStarted = false;
  triggerPin = triggerPinArg;
  currentSettings.mode = LONG_EXPOSURE_STILL;
  currentSettings.exposures = 1;
  currentSettings.delay_time = 1;
  currentSettings.pre_delay_time = 1;
  currentSettings.exposure_time = 1;
  currentSettings.pan_angle = 0.0;
  currentSettings.panDirection = true;
  currentSettings.dither = false;
  currentSettings.dither_frequency = 1;
  currentSettings.post_tracking_on = false;
  currentSettings.frames = 1;
  currentSettings.pixel_size = 1.0;
  currentSettings.focal_length = 1;
}

/* MODES:
LONG_EXPOSURE_STILL: Predelay>(Internal Timed Capture>Dither?>Delay (x exposures)) (with tracking)
LONG_EXPOSURE_MOVIE: (Predelay>(Internal Timed Capture>Dither?>Delay (x exposures))>rewind axis (x frames)) (with tracking)
DAY_TIME_LAPSE: Predelay>(Camera Capture>delay (x exposures)) (no tracking)
DAY_TIME_LAPSE_PAN: Predelay>(Camera Capture>pan deg>delay (x exposures)) (no tracking)
*/

void Intervalometer::run() {
  switch (currentState) {
    case INACTIVE:
      intervalometerActive = false;
      exposures_taken = 0;
      frames_taken = 0;
      if (!currentSettings.post_tracking_on) {
        ra_axis.stopTracking();
      }
      break;
    case PRE_DELAY:
      if (!timerStarted) {
        if ((currentSettings.mode == DAY_TIME_LAPSE || currentSettings.mode == DAY_TIME_LAPSE_PAN) && ra_axis.trackingActive) {
          ra_axis.stopTracking();
        } else if ((currentSettings.mode == LONG_EXPOSURE_MOVIE || currentSettings.mode == LONG_EXPOSURE_STILL) && !ra_axis.trackingActive) {
          sendError("TRACKING NOT ACTIVE!! Please start");
          currentState = INACTIVE;
          break;
        }
        ra_axis.counterActive = currentSettings.mode == LONG_EXPOSURE_MOVIE ? true : false;
        intervalometerTimer.start(2000 * currentSettings.pre_delay_time, false);
        timerStarted = false;
        intervalometerActive = true;
      }
      if (nextState) {
        nextState = false;
        timerStarted = false;
        currentState = CAPTURE;
      }
      break;
    case CAPTURE:  //add capture for day time

      if (!timerStarted) {
        if (currentSettings.mode == LONG_EXPOSURE_MOVIE) {
          ra_axis.resetAxisCount();
          ra_axis.counterActive = true;
        }

        digitalWrite(triggerPin, HIGH);
        intervalometerTimer.start(2000 * currentSettings.exposure_time, false);
        timerStarted = true;
      }
      if (nextState) {
        digitalWrite(triggerPin, LOW);
        nextState = false;
        timerStarted = false;
        exposures_taken++;
        currentState = currentSettings.dither ? DITHER : currentSettings.mode == DAY_TIME_LAPSE_PAN ? PAN
                                                                                                    : DELAY;
      }
      break;
    case DITHER:
      if (exposures_taken % currentSettings.dither_frequency == 0) {
        runDither();  //program dither.
        currentState = DELAY;
      }
      break;
    case PAN:
      if (!ra_axis.goToTarget) {
        uint64_t arcSecsToMove = uint64_t(3600.0 * currentSettings.pan_angle);
        int64_t stepsToMove = currentSettings.panDirection ? arcSecsToMove / ARCSEC_PER_STEP : (arcSecsToMove / ARCSEC_PER_STEP) * -1;
        ra_axis.resetAxisCount();
        ra_axis.setAxisTargetCount(stepsToMove);
        ra_axis.counterActive = ra_axis.goToTarget = true;
        ra_axis.startSlew(ra_axis.tracking_rate / 5, currentSettings.panDirection);  //pan at 10x tracking rate
      }
      if (!ra_axis.slewActive) {
        currentState = DELAY;
      }
      break;
    case DELAY:
      if (exposures_taken >= currentSettings.exposures) {
        currentState = currentSettings.mode != LONG_EXPOSURE_MOVIE ? INACTIVE : frames_taken < currentSettings.frames ? REWIND
                                                                                                                      : INACTIVE;
      } else {
        if (!timerStarted) {
          intervalometerTimer.start(2000 * currentSettings.delay_time, false);
          timerStarted = true;
        }
        if (nextState) {
          nextState = false;
          timerStarted = false;
          currentState = CAPTURE;
        }
      }

      break;
    case REWIND:
      if (!ra_axis.goToTarget) {
        exposures_taken = 0;
        frames_taken++;
        ra_axis.setAxisTargetCount(0);
        ra_axis.goToTarget = true;
        ra_axis.startSlew(ra_axis.tracking_rate / 5, !ra_axis.trackingDirection);  //rewind at 10 x tracking rate.
        exposures_taken = 0;
        frames_taken++;
      }
      if (!ra_axis.slewActive) {
        currentState = CAPTURE;
      }
      break;
  }
}

void Intervalometer::saveSettingsToPreset(uint8_t preset) {
  presets[preset] = currentSettings;
  savePresetsToEEPPROM();
}

void Intervalometer::readSettingsFromPreset(uint8_t preset) {
  currentSettings = presets[preset];
}

void Intervalometer::savePresetsToEEPPROM() {
  writeObjectToEEPROM(PRESETS_EEPROM_START_LOCATION, presets);
}

void Intervalometer::readPresetsFromEEPROM() {
  readObjectFromEEPROM(PRESETS_EEPROM_START_LOCATION, presets);
}

uint16_t Intervalometer::getStepsPerTenPixels() {
  //add 0.5 to round up float to nearest int while casting
  return (int)(((getArcsec_per_pixel() * 10.0) / ARCSEC_PER_STEP) + 0.5);
}

float Intervalometer::getArcsec_per_pixel() {
  //div pixel size by 100 since we multiplied it by 100 in html page
  return (((float)currentSettings.pixel_size / 100.0) / currentSettings.focal_length) * 206.265;
}

void Intervalometer::runDither() {
  //pass
}

void ditherRoutine() {
  int i = 0, j = 0;
  timerAlarmDisable(timer_tracking);
  int random_direction = biased_random_direction(previous_direction);
  previous_direction = random_direction;
  digitalWrite(AXIS1_DIR, random_direction);  //dither in a random direction
  delay(500);

  for (i = 0; i < dither_intensity; i++) {
    for (j = 0; j < steps_per_10pixels; j++) {
      digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));
      delay(10);
      digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));
      delay(10);
    }
  }

  delay(1000);
  initTracking();
  delay(3000);  //settling time after dither
}

// when tracker moves left, next time its 5% higher chance tracked will move right
// with this tracker should keep in the middle in average
int Intervalometer::biased_random_direction(int previous_direction) {
  // Adjust probabilities based on previous selection
  if (previous_direction == 0) {
    direction_left_bias = 0.45;   // Lower probability for 0
    direction_right_bias = 0.55;  // Higher probability for 1
  }
  if (previous_direction == 1) {
    direction_left_bias = 0.55;   // Higher probability for 0
    direction_right_bias = 0.45;  // Lower probability for 1
  }

  float rand_val = random(100) / 100.0;  // random number between 0.00 and 0.99

  if (rand_val < direction_left_bias) {
    return 0;
  } else {
    return 1;
  }
}

template<class T> int Intervalometer::writeObjectToEEPROM(int address, const T& object) {
  const byte* p = (const byte*)(const void*)&object;
  unsigned int i;
  for (i = 0; i < sizeof(object); i++) {
    EEPROM.write(address++, *p++);
  }
  EEPROM.commit();
  return i;
}

template<class T> int Intervalometer::readObjectFromEEPROM(int address, T& object) {
  byte* p = (byte*)(void*)&object;
  unsigned int i;
  for (i = 0; i < sizeof(object); i++)
    *p++ = EEPROM.read(address++);
  return i;
}
