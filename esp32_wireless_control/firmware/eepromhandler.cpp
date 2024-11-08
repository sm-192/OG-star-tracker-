// #include "eepromhandler.h"


// template<class T> int writeObjectToEEPROM(int address, const T& object) {
//   const byte* p = (const byte*)(const void*)&object;
//   unsigned int i;
//   for (i = 0; i < sizeof(object); i++)
//     EEPROM.write(address++, *p++);
//   return i;
// }

// template<class T> int readObjectFromEEPROM(int address, T& object) {
//   byte* p = (byte*)(void*)&object;
//   unsigned int i;
//   for (i = 0; i < sizeof(object); i++)
//     *p++ = EEPROM.read(address++);
//   return i;
// }