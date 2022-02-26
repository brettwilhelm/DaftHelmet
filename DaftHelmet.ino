/* Daft Helmet - v1.0.0
   Brett Wilhelm - brett@fraggle.net
   ESP 32/8266 Firmware for Daft Helmet visor display.
*/

/* Header Files */
//#include <WiFi.h> /* ESP32 */
#include <ESP8266WiFi.h> /* ESP8266 */
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
/* Project Specific */
#include "Text.h"
#include "Settings.h"
#include "Animations.h"
#include "Functions.h"
#include "WebServer.h"

/* Arduino setup and loop */
void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  #ifdef DEBUG
    logger("Configuring AP.");
  #endif
  WiFi.softAP(ap_ssid, ap_password);

  #ifdef DEBUG
    logger("Starting Web Server.");
  #endif
  server.begin();

  #ifdef BUTTONS
    #ifdef DEBUG
      logger("Configuring Buttons.");
    #endif
    pinMode(ButtonOnOff, INPUT_PULLUP);
    pinMode(ButtonMode, INPUT_PULLUP);
  #endif

  Matrix[MatrixLeft].begin(0x70);
  Matrix[MatrixRight].begin(0x71);
  MatrixInitCoordinates();
}

void loop() {
  client = server.available();
  if (client) { httpd_client(client); }

  if (digitalRead(ButtonOnOff) == ButtonOnOffClosed) {
    MatrixSetIsOn(!isOn);
    while (digitalRead(ButtonOnOff) == ButtonOnOffClosed) { delay(50); }
  }

  if (digitalRead(ButtonMode) == ButtonModeClosed) {
    MatrixAllOn();
    while (digitalRead(ButtonMode) == ButtonModeClosed) { delay(50); }
    MatrixClear(true);
    return;
  }
  else if (!isOn) { return; }
  
  MatrixClear();
  MatrixDisplay();
  delay(my_speed);
}
