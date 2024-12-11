#include "web_languages.h"
// // Parameter key definitions
// const ParamKeyTable paramKeys[] = {
//     {.key = "trackingSpeed",  //STR_TRACKING_SPEED},
//     {.key = "speed",  //STR_SPEED},
//     {.key = "direction",  //STR_DIRECTION},
//     {.key = "exposure",  //STR_EXPOSURE},
//     {.key = "numExposures",  //STR_NUM_EXPOSURES},
//     {.key = "ditherEnabled",  //STR_DITHER_ENABLED},
//     {.key = "focalLength",  //STR_FOCAL_LENGTH},
//     {.key = "pixSize",  //STR_PIXEL_SIZE},
//     {.key = "disableTracking",  //STR_DISABLE_TRACKING},
// };

const char *langs[] = {  //add array pointers for additional languages and update Languages enum in web_language.h
  *englishLangStrings,
  *chineseLangStrings
};

const char *englishLangStrings[] = {
  "Tracking ON",                    //STR_TRACKING_ON
  "Tracking OFF",                   //STR_TRACKING_OFF
  "Idle",                           //STR_IDLE
  "Slewing",                        //STR_SLEWING
  "Invalid Exposure Values!",       //STR_INVALID_EXPOSURE
  "Invalid Dither Settings!",       //STR_INVALID_DITHER
  "Capture Started",                //STR_CAPTURE_ON
  "Capture Stopped",                //STR_CAPTURE_OFF
  "Capture Already Running",        //STR_CAPTURE_ALREADY_ON
  "Capture Already Stopped",        //STR_CAPTURE_ALREADY_OFF
  "%d Captures Remaining...",       //STR_CAPTURES_REMAINING
  "OG Star Tracker Control Panel",  //STR_TITLE
  "Tracking",                       //STR_TRACKING
  "Hemisphere",                     //STR_HEMISPHERE
  "North",                          //STR_NORTH
  "South",                          //STR_SOUTH
  "Tracking Rate",                  //STR_TRACKING_RATE
  "Sidereal",                       //STR_SIDEREAL
  "Solar",                          //STR_SOLAR
  "Lunar",                          //STR_LUNAR
  "Slew Control",                   //STR_SLEW_CONTROL
  "Speed Multiplier",               //STR_SPEED_MULTIPLIER
  "Custom",                         //STR_CUSTOM
  "Custom Speed",                   //STR_CUSTOM_SPEED
  "Hold direction button to slew",  //STR_SLEW_HINT
  "Slew Left",                      //STR_SLEW_LEFT
  "Slew Right",                     //STR_SLEW_RIGHT
  "Abort Slew",                     //STR_ABORT_SLEW
  "Photo Control",                  //STR_PHOTO_CONTROL
  "Exposure Length",                //STR_EXPOSURE_LENGTH
  "in seconds (Ex. 30)",            //STR_EXPOSURE_HINT
  "number of photos (Ex. 20)",      //STR_NUM_EXPOSURES_HINT
  "Dither Settings",                //STR_DITHER_SETTINGS
  "Enable Dithering",               //STR_DITHER_ENABLE
  "in millimeters (Ex. 135)",       //STR_FOCAL_LENGTH_HINT
  "in micrometers (Ex. 4.1)",       //STR_PIXEL_SIZE_HINT
  "Start Capture",                  //STR_START_CAPTURE
  "Abort Capture",                  //STR_ABORT_CAPTURE
  "Status",                         //STR_STATUS
  "Ready",                          //STR_STATUS_MSG
  "Firmware Version",               //STR_FIRMWARE_VERSION
  "ON",                             //STR_BTN_ON
  "OFF",                            //STR_BTN_OFF
};

const char *chineseLangStrings[]{

  // Chinese strings
  "追踪开启",                  //STR_TRACKING_ON
  "追踪关闭",                  //STR_TRACKING_OFF
  "空闲",                      //STR_IDLE
  "快速调整中",                //STR_SLEWING
  "无效的曝光参数!",           //STR_INVALID_EXPOSURE
  "无效的抖动设置!",           //STR_INVALID_DITHER
  "拍摄开始",                  //STR_CAPTURE_ON
  "拍摄停止",                  //STR_CAPTURE_OFF
  "拍摄已在进行中",            //STR_CAPTURE_ALREADY_ON
  "拍摄已经停止",              //STR_CAPTURE_ALREADY_OFF
  "剩余拍摄次数: %d",          //STR_CAPTURES_REMAINING
  "OG 赤道仪控制面板",         //STR_TITLE
  "追踪",                      //STR_TRACKING
  "半球",                      //STR_HEMISPHERE
  "北半球",                    //STR_NORTH
  "南半球",                    //STR_SOUTH
  "追踪速率",                  //STR_TRACKING_RATE
  "恒星速率",                  //STR_SIDEREAL
  "太阳速率",                  //STR_SOLAR
  "月球速率",                  //STR_LUNAR
  "快速调",                    //STR_SLEW_CONTROL
  "速度倍率",                  //STR_SPEED_MULTIPLIER
  "自定义",                    //STR_CUSTOM
  "自定义速度",                //STR_CUSTOM_SPEED
  "按住方向按钮进行快速调整",  //STR_SLEW_HINT
  "向左调整",                  //STR_SLEW_LEFT
  "向右调整",                  //STR_SLEW_RIGHT
  "停止调整",                  //STR_ABORT_SLEW
  "拍摄控制",                  //STR_PHOTO_CONTROL
  "曝光时长",                  //STR_EXPOSURE_LENGTH
  "单位：秒 (例如: 30)",       //STR_EXPOSURE_HINT
  "拍摄张数 (例如: 20)",       //STR_NUM_EXPOSURES_HINT
  "抖动设置",                  //STR_DITHER_SETTINGS
  "启用抖动",                  //STR_DITHER_ENABLE
  "单位：毫米 (例如: 135)",    //STR_FOCAL_LENGTH_HINT
  "单位：微米 (例如: 4.1)",    //STR_PIXEL_SIZE_HINT
  "开始拍摄",                  //STR_START_CAPTURE
  "停止拍摄",                  //STR_ABORT_CAPTURE
  "状态",                      //STR_STATUS
  "就绪",                      //STR_STATUS_MSG
  "固件版本",                  //STR_FIRMWARE_VERSION
  "开启",                      //STR_BTN_ON
  "关闭",                      //STR_BTN_OFF
  "中文",                      //STR_LANG_CN
  "中文",                      //STR_LANG_CN
};

const char *nextLangStrings[] = {
  "",  //STR_TRACKING_ON
  "",  //STR_TRACKING_OFF
  "",  //STR_IDLE
  "",  //STR_SLEWING
  "",  //STR_INVALID_EXPOSURE
  "",  //STR_INVALID_DITHER
  "",  //STR_CAPTURE_ON
  "",  //STR_CAPTURE_OFF
  "",  //STR_CAPTURE_ALREADY_ON
  "",  //STR_CAPTURE_ALREADY_OFF
  "",  //STR_CAPTURES_REMAINING
  "",  //STR_TITLE
  "",  //STR_TRACKING
  "",  //STR_HEMISPHERE
  "",  //STR_NORTH
  "",  //STR_SOUTH
  "",  //STR_TRACKING_RATE
  "",  //STR_SIDEREAL
  "",  //STR_SOLAR
  "",  //STR_LUNAR
  "",  //STR_SLEW_CONTROL
  "",  //STR_SPEED_MULTIPLIER
  "",  //STR_CUSTOM
  "",  //STR_CUSTOM_SPEED
  "",  //STR_SLEW_HINT
  "",  //STR_SLEW_LEFT
  "",  //STR_SLEW_RIGHT
  "",  //STR_ABORT_SLEW
  "",  //STR_PHOTO_CONTROL
  "",  //STR_EXPOSURE_LENGTH
  "",  //STR_EXPOSURE_HINT
  "",  //STR_NUM_EXPOSURES_HINT
  "",  //STR_DITHER_SETTINGS
  "",  //STR_DITHER_ENABLE
  "",  //STR_FOCAL_LENGTH_HINT
  "",  //STR_PIXEL_SIZE_HINT
  "",  //STR_START_CAPTURE
  "",  //STR_ABORT_CAPTURE
  "",  //STR_STATUS
  "",  //STR_STATUS_MSG
  "",  //STR_FIRMWARE_VERSION
  "",  //STR_BTN_ON
  "",  //STR_BTN_OFF
  "",  //STR_LANG_CN
  "",  //STR_LANG_CN

};