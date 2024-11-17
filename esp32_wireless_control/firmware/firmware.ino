#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <string.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include "config.h"
#include "website_strings.h"
#include "intervalometer.h"
#include "axis.h"
#include "hardwaretimer.h"
#include "index_html.h"

// try to update every time there is breaking change
const int firmware_version = 6;

// Set your Wi-Fi credentials
const byte DNS_PORT = 53;
//const char* ssid = "OG Star Tracker";  //change to your SSID
//const char* password = "password123";  //change to your password, must be 8+ characters
const char* ssid = "NowhereNet";  //change to your SSID
const char* password = "Ilove69dogsand34*cats";  //change to your password, must be 8+ characters
//If you are using AP mode, you can access the website using the below URL
const String website_name = "www.tracker.com";
#define MIN_CUSTOM_SLEW_RATE 2

unsigned long blink_millis = 0;

WebServer server(80);
DNSServer dnsServer;

// Handle requests to the root URL ("/")
void handleRoot() {
  server.send(200, MIME_TYPE_HTML, html_content);
}

void handleOn() {
  int tracking_speed = server.arg(TRACKING_SPEED).toInt();
  TRACKING_RATES rate;
  switch (tracking_speed) {
    case 0:  //sidereal rate
      rate = TRACKING_SIDEREAL;
      break;
    case 1:  //solar rate
      rate = TRACKING_SOLAR;
      break;
    case 2:  //lunar rate
      rate = TRACKING_LUNAR;
      break;
    default:
      rate = TRACKING_SIDEREAL;
      break;
  }
  int direction = server.arg(DIRECTION).toInt();
  ra_axis.startTracking(rate, direction);
  server.send(200, MIME_TYPE_TEXT, TRACKING_ON);
}

void handleOff() {
  ra_axis.stopTracking();
  server.send(200, MIME_TYPE_TEXT, TRACKING_OFF);
}

void handleSlewRequest() { //add togther with 
  if (!ra_axis.slewActive) {  //if slew is not active - needed for ipad (passes multiple touchon events)
    int slew_speed = server.arg(SPEED).toInt();
    int slew_direction = server.arg(SLEW_DIRECTION).toInt();
    //limit custom slew speed to 2-400
    slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE                                                                                                          : slew_speed;
    ra_axis.startSlew((2 * ra_axis.tracking_rate) / slew_speed, slew_direction);
    server.send(200, MIME_TYPE_TEXT, SLEWING);
  }
}

void handleLeft() { //add togther with 
  if (!ra_axis.slewActive) {  //if slew is not active - needed for ipad (passes multiple touchon events)
    int slew_speed = server.arg(SPEED).toInt();
    //limit custom slew speed to 2-400
    slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE                                                                                                          : slew_speed;
    ra_axis.startSlew((2 * ra_axis.tracking_rate) / slew_speed, 0);
    server.send(200, MIME_TYPE_TEXT, SLEWING);
  }
}

void handleRight() {
  if (!ra_axis.slewActive) {  //if slew is not active - needed for ipad (passes multiple touchon events)
    int slew_speed = server.arg(SPEED).toInt();
    //limit custom slew speed to 2-400
    slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                                                                              : slew_speed;
    ra_axis.startSlew((2 * ra_axis.tracking_rate) / slew_speed, 1);
    server.send(200, MIME_TYPE_TEXT, SLEWING);
  }
}

void handleSlewOff() {
  if (ra_axis.slewActive) {  //if slew is active needed for ipad (passes multiple touchoff events)
    ra_axis.stopSlew();
  }
}

void handleSetExposureSettings() {
  //include sever Args for preset
  JsonDocument settings;
  String json;
  settings["mode"] = intervalometer.currentSettings.mode;
  settings["exposures"] = intervalometer.currentSettings.exposures;
  settings["delay_time"] = intervalometer.currentSettings.delay_time;
  settings["pre_delay_time"] = intervalometer.currentSettings.pre_delay_time;
  settings["exposure_time"] = intervalometer.currentSettings.exposure_time;
  settings["pan_angle"] = intervalometer.currentSettings.pan_angle;
  settings["panDirection"] = intervalometer.currentSettings.panDirection;
  settings["dither"] = intervalometer.currentSettings.dither;
  settings["dither_frequency"] = intervalometer.currentSettings.dither_frequency;
  settings["post_tracking_on"] = intervalometer.currentSettings.post_tracking_on;
  settings["frames"] = intervalometer.currentSettings.frames;
  settings["pixel_size"] = intervalometer.currentSettings.pixel_size;
  settings["focal_length"] = intervalometer.currentSettings.focal_length;
  serializeJson(settings, json);
  Serial.println(json);
  //intervalometer.readSettingsFromPreset(preset);
  //server.send
}


void handleGetPresetExposureSettings() {
  int preset; //get server args
  intervalometer.readSettingsFromPreset(preset);
  JsonDocument settings;
  String json;
  settings["mode"] = intervalometer.currentSettings.mode;
  settings["exposures"] = intervalometer.currentSettings.exposures;
  settings["delay_time"] = intervalometer.currentSettings.delay_time;
  settings["pre_delay_time"] = intervalometer.currentSettings.pre_delay_time;
  settings["exposure_time"] = intervalometer.currentSettings.exposure_time;
  settings["pan_angle"] = intervalometer.currentSettings.pan_angle;
  settings["panDirection"] = intervalometer.currentSettings.panDirection;
  settings["dither"] = intervalometer.currentSettings.dither;
  settings["dither_frequency"] = intervalometer.currentSettings.dither_frequency;
  settings["post_tracking_on"] = intervalometer.currentSettings.post_tracking_on;
  settings["frames"] = intervalometer.currentSettings.frames;
  settings["pixel_size"] = intervalometer.currentSettings.pixel_size;
  settings["focal_length"] = intervalometer.currentSettings.focal_length;
  serializeJson(settings, json);
  Serial.println(json);
  //server.send
}

void handleStartCapture() {
}

void handleAbortCapture() {
}

void handleStatusRequest() {  //tosort
  if (ra_axis.slewActive) {
    slewTimeOut.setCountValue(0);  //reset timeout counter
    server.send(200, MIME_TYPE_TEXT, SLEWING);
    // } else if (photo_control_status != INACTIVE) {
    //   char status[60];
    //   sprintf(status, CAPTURES_REMAINING, exposure_count - exposures_taken);
    //   server.send(200, MIME_TYPE_TEXT, status);
    //   return;
    // } else if (!ra_axis.trackingActive && photo_control_status == INACTIVE) {
    //   server.send(200, MIME_TYPE_TEXT, IDLE);
    //   return;
  } else if (ra_axis.trackingActive) {
    server.send(200, MIME_TYPE_TEXT, TRACKING_ON);
    return;
  }

  server.send(204, MIME_TYPE_TEXT, "dummy");

  // TODO add detection for capturing
}

void sendError(String errorMessage) {
  server.send(204, MIME_TYPE_TEXT, errorMessage);
}

void handleVersion() {
  server.send(200, MIME_TYPE_TEXT, (String)firmware_version);
}

void setup() {
  Serial.begin(115200);

#ifdef AP
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP(ssid, password);
  delay(500);
  Serial.println("Creating Wifi Network");

  //ANDROID 10 WORKAROUND==================================================
  //set new WiFi configurations
  WiFi.disconnect();
  Serial.println("reconfig WiFi...");
  /*Stop wifi to change config parameters*/
  esp_wifi_stop();
  esp_wifi_deinit();
  /*Disabling AMPDU RX is necessary for Android 10 support*/
  wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();  //We use the default config ...
  my_config.ampdu_rx_enable = 0;                              //... and modify only what we want.
  Serial.println("WiFi: Disabled AMPDU...");
  esp_wifi_init(&my_config);  //set the new config = "Disable AMPDU"
  esp_wifi_start();           //Restart WiFi
  delay(500);
  //ANDROID 10 WORKAROUND==================================================
#else
  WiFi.mode(WIFI_MODE_STA);  // Set ESP32 in station mode
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Network in STA mode");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
#endif
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, website_name, WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleOn);
  server.on("/off", HTTP_GET, handleOff);
  server.on("/startslew", HTTP_GET, handleSlewRequest);
  server.on("/left", HTTP_GET, handleLeft);
  server.on("/right", HTTP_GET, handleRight);
  server.on("/stopslew", HTTP_GET, handleSlewOff);
  server.on("/start", HTTP_GET, handleStartCapture);
  server.on("/abort", HTTP_GET, handleAbortCapture);
  server.on("/status", HTTP_GET, handleStatusRequest);
  server.on("/version", HTTP_GET, handleVersion);

  // Start the server
  server.begin();

#ifdef AP
  Serial.println(WiFi.softAPIP());
#else
  Serial.println(WiFi.localIP());
#endif
  pinMode(INTERV_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(AXIS1_STEP, OUTPUT);
  pinMode(AXIS1_DIR, OUTPUT);
  pinMode(EN12_n, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  digitalWrite(AXIS1_STEP, LOW);
  digitalWrite(EN12_n, LOW);
  //handleExposureSettings();
}

void loop() {
  if (ra_axis.slewActive) {
    //blink status led if mount is in slew mode
    if (millis() - blink_millis >= 150) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      blink_millis = millis();
    }
  } else {
    //turn on status led if sidereal tracking is ON
    digitalWrite(STATUS_LED, ra_axis.trackingActive);
  }

  if (intervalometer.intervalometerActive) {
    intervalometer.run();
  }

  server.handleClient();
  dnsServer.processNextRequest();
}