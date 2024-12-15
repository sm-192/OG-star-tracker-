#ifndef WEB_LANGUAGES_H
#define WEB_LANGUAGES_H


#define numberOfHTMLStrings 32

enum Languages {
  EN,
  CN,
  // Add additional languages
  LANG_COUNT
};



enum Messages {
  STR_TRACKING_ON,
  STR_TRACKING_OFF,
  STR_IDLE,
  STR_SLEWING,
  STR_INVALID_EXPOSURE,
  STR_INVALID_DITHER,
  STR_CAPTURE_ON,
  STR_CAPTURE_OFF,
  STR_CAPTURE_ALREADY_ON,
  STR_CAPTURE_ALREADY_OFF,
  STR_CAPTURES_REMAINING,
  NUMBER_OF_MESSAGES
};



extern const char** languageNames[];
extern const char** languageMessageStrings[];
extern const char** languageHTMLStrings[];


























#endif