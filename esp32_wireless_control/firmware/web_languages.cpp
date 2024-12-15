#include "web_languages.h"
#include "englishLanguage.h"
#include "chineseLanguage.h"



const char** languageNames[LANG_COUNT] = {
  englishLangLanguageNames,
  chineseLangLanguageNames
};

const char** languageMessageStrings[LANG_COUNT] = {
  englishLangMessageStrings,
  chineseLangMessageStrings
};

const char** languageHTMLStrings[LANG_COUNT] = {
  englishLangHTMLStrings,
  chineseLangHTMLStrings
};

const char* HTMLplaceHolders[numberOfHTMLStrings] = {
  "%STR_TITLE%",
  "%STR_TRACKING%",
  "%STR_HEMISPHERE%",
  "%STR_NORTH%",
  "%STR_SOUTH%",
  "%STR_TRACKING_RATE%",
  "%STR_SIDEREAL%",
  "%STR_SOLAR%",
  "%STR_LUNAR%",
  "%STR_SLEW_CONTROL%",
  "%STR_SPEED_MULTIPLIER%",
  "%STR_CUSTOM%",
  "%STR_CUSTOM_SPEED%",
  "%STR_SLEW_HINT%",
  "%STR_SLEW_LEFT %",
  "%STR_SLEW_RIGHT %",
  "%STR_ABORT_SLEW%",
  "%STR_PHOTO_CONTROL%",
  "%STR_EXPOSURE_LENGTH%",
  "%STR_EXPOSURE_HINT%",
  "%STR_NUM_EXPOSURES_HINT%",
  "%STR_DITHER_SETTINGS%",
  "%STR_DITHER_ENABLE %",
  "%STR_FOCAL_LENGTH_HINT%",
  "%STR_PIXEL_SIZE_HINT%",
  "%STR_START_CAPTURE%",
  "%STR_ABORT_CAPTURE%",
  "%STR_STATUS%",
  "%STR_STATUS_MSG%",
  "%STR_FIRMWARE_VERSION%",
  "%STR_BTN_ON%",
  "%STR_BTN_OFF%"
};
