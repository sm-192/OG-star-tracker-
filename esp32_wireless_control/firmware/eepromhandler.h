#ifndef EEPROMHANDLER_H
#define EEPROMHANDLER_H

#include <EEPROM.h>
#include "config.h"

  template<class T> int writeObjectToEEPROM(int address, const T& object);
  template<class T> int readObjectFromEEPROM(int address, T& object);

#endif