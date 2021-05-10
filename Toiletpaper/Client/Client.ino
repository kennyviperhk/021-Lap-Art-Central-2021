/*
   WebSocketClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>
#include <AccelStepper.h>
#include <Hash.h>
#include "config.h"

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define USE_SERIAL Serial


int x = 0, y = 0, z = 0, s = 1000;
int currX = 0, currY = 0, currZ = 0;
long currentMillis;
long prevMillis;


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

        // send message to server when Connected
        //webSocket.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[WSc] get text: %s\n", payload);
      decode_text((char*)payload);
      // send message to server
      //webSocket.sendTXT("123");
      break;
  }

}


void setup() {
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);

  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP(SSID, PASSWORD);

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  // server address, port and URL
  webSocket.begin("192.168.1.1", 80, "/ws");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  webSocket.setAuthorization("admin", "admin");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
  motorSetup();
}

void loop() {
  currentMillis = millis();
  webSocket.loop();
  motorLoop();
}

void decode_text(String s) {
  int colorCodeBegin = s.indexOf('#');
  String c = s.substring(colorCodeBegin + 1, s.length());

  // we get stepper data
  if (c.substring(0, 1) == "x") {
    char* buf = (char *)c.c_str();
    int n = sscanf(buf, "x%dy%dz%d", &x, &y, &z);
    if (y > 0) {
      currY += 50;
    }
    if (y < 0) {
      currY -= 50;
    }
  }
  if (c.substring(0, 1) == "s") {
    char* buf = (char *)c.c_str();
    int n = sscanf(buf, "s%d", &s);
  }
}
