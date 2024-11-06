 #include "intervalometer.h"
 
 //     arcsec_per_pixel = (((float)pixel_size / 100.0) / focal_length) * 206.265;        //div pixel size by 100 since we multiplied it by 100 in html page
  //     steps_per_10pixels = (int)(((arcsec_per_pixel * 10.0) / arcsec_per_step) + 0.5);  //add 0.5 to round up float to nearest int while casting
void intervalometer_ISA() {
  //pass
}


Intervalometer::Intervalometer(uint8_t triggerPin) : intervalometerTimer(40000) { //check this frequency
  currentMode = LONG_EXPOSURE_STILL;
  currentState = INACTIVE;
  nextState = false;
  intervalometerActive = false;
  readPresetsFromEEPROM();

}

void Intervalometer::saveSettingsToPreset(uint8_t preset) {
  presets[preset] = currentSettings;
  savePresetsToEEPPROM();
}

void Intervalometer::readSettingsFromPresets(uint8_t preset) {
   currentSettings = presets[preset];
}

void Intervalometer::savePresetsToEEPPROM() {
  writeObjectToEEPROM(PRESETS_EEPROM_START_LOCATION, presets);
}

void Intervalometer::readPresetsFromEEPROM() {
  readObjectFromEEPROM(PRESETS_EEPROM_START_LOCATION, presets);
}

