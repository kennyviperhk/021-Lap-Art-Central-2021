#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <AccelStepper.h>
#include "config.h"



AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

//flag to use from web update to reboot the ESP
bool shouldReboot = false;

AccelStepper stepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
int x = 0, y = 0, z = 0, s = 100, m = 1;
long currX = 0, currY = 0, currZ = 0;
long interval = 5000;
int moveVal = 100;
long limit = 10000;
long currentMillis;
long prevMillis;

void onRequest(AsyncWebServerRequest *request) {
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  //Handle upload
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  //Handle WebSocket event
  if (type == WS_EVT_CONNECT) {
    //    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    //client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if (type == WS_EVT_DISCONNECT) {
    //    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if (type == WS_EVT_ERROR) {
    //    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if (type == WS_EVT_PONG) {
    //    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char*)data : "");
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if (info->final && info->index == 0 && info->len == len) {
      //the whole message is in a single frame and we got all of it's data
      //      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

      if (info->opcode == WS_TEXT) {
        for (size_t i = 0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for (size_t i = 0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      //      Serial.printf("%s\n", msg.c_str());

      if (info->opcode == WS_TEXT) {


        decode_text(msg);
      }
      else {
        //client->binary("I got your binary message");
      }
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0) {
        if (info->num == 0) {
          //          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          //        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
        }
      }
      //      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

      if (info->opcode == WS_TEXT) {
        for (size_t i = 0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for (size_t i = 0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      //      Serial.printf("%s\n", msg.c_str());

      if ((info->index + len) == info->len) {
        //        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final) {
          //          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT) {
            //client->text("I got your text message" + msg);
          }
          else {
            //client->binary("I got your binary message" + msg);
          }
        }
      }
    }
  }
}
IPAddress    apIP(192, 168, 1, 1);  // Defining a static IP address: local & gateway
void setup() {
  Serial.begin(115200);
  Serial.println("Connect WIFI");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(SSID, PASSWORD);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // attach AsyncEventSource
  server.addHandler(&events);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<html><head><script>;var connection=new WebSocket('ws://'+location.hostname+'/ws',['arduino']);connection.onopen=function(){connection.send('Connect '+new Date())};connection.onerror=function(n){console.log('WebSocket Error ',n)};connection.onmessage=function(n){console.log('Server: ',n.data)};connection.onclose=function(n){console.log('Socket is closed. Reconnect will be attempted in 1 second.',n.reason);setTimeout(function(){connection=new WebSocket('ws://'+location.hostname+'/ws',['arduino'])},1000)};var s=100;function updateSpeed(){s=parseInt(document.getElementById('speed').value)};function sendSpeed(){var n='#s'+s;console.log('sendSpeed: '+n);connection.send(n)};function sendStepper(n,o,c){var e='#x'+n+'y'+o+'z'+c;console.log('sendStepper: '+e);connection.send(e)};function sendMode(n){var e='#m'+n;console.log('sendMode: '+e);connection.send(e)};</script></head><body> Toilet Paper 1: <br /><br /><button onclick=\"sendStepper(-1,0,0);\" /><<<</button>   <button onclick=\"sendStepper(1,0,0);\" />>>></button><br /><br /> Toilet Paper 2: <br /><br /><button onclick=\"sendStepper(0,-1,0);\" /><<<</button>   <button onclick=\"sendStepper(0,1,0);\" />>>></button><br /><br /> Toilet Paper 3: <br /><br /><button onclick=\"sendStepper(0,0,-1);\" /><<<</button>   <button onclick=\"sendStepper(0,0,1);\" />>>></button><br /><br /> Speed: <input id=\"speed\" type=\"range\" min=\"100\" max=\"3000\" step=\"100\" oninput=\"updateSpeed();\" onmouseup=\"sendSpeed();\" ontouchend=\"sendSpeed();\"/><br /><br /><button onclick=\"sendMode(0);\" /> Manual Mode </button>   <button onclick=\"sendMode(1);\" /> Auto Mode </button></body></html>");
  });


  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  // upload a file to /upload
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  }, onUpload);

  // send a file when /index is requested
  server.on("/index", HTTP_ANY, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.htm");
  });

  // Simple Firmware Update Form
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest * request) {
    shouldReboot = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "OK" : "FAIL");
    response->addHeader("Connection", "close");
    request->send(response);
  }, [](AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      Serial.printf("Update Start: %s\n", filename.c_str());
      Update.runAsync(true);
      if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
        Update.printError(Serial);
      }
    }
    if (!Update.hasError()) {
      if (Update.write(data, len) != len) {
        Update.printError(Serial);
      }
    }
    if (final) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %uB\n", index + len);
      } else {
        Update.printError(Serial);
      }
    }
  });

  // attach filesystem root at URL /fs
  server.serveStatic("/fs", SPIFFS, "/");

  // Catch-All Handlers
  // Any request that can not find a Handler that canHandle it
  // ends in the callbacks below.
  server.onNotFound(onRequest);
  server.onFileUpload(onUpload);
  server.onRequestBody(onBody);

  server.begin();
  motorSetup();
}

void loop() {
  if (shouldReboot) {
    Serial.println("Rebooting...");
    delay(100);
    ESP.restart();
  }
  static char temp[128];
  sprintf(temp, "Seconds since boot: %u", millis() / 1000);
  currentMillis = millis();
  motorLoop();
}

void decode_text(String s) {
  ws.textAll(s);
  int colorCodeBegin = s.indexOf('#');
  String c = s.substring(colorCodeBegin + 1, s.length());

  // we get stepper data
  if (c.substring(0, 1) == "s") {
    char* buf = (char *)c.c_str();
    int n = sscanf(buf, "s%d", &s);
  }
  if (c.substring(0, 1) == "m") {
    char* buf = (char *)c.c_str();
    int n = sscanf(buf, "m%d", &m);
  }
  if (m == 0) {
    if (c.substring(0, 1) == "x") {
      char* buf = (char *)c.c_str();
      int n = sscanf(buf, "x%dy%dz%d", &x, &y, &z);
      if (x > 0) {
        currX += moveVal;
      }
      if (x < 0) {
        currX -= moveVal;
      }
      if (currX > 10000) {
        currX = 10000;
      }
      if (currX < -10000) {
        currX = -10000;
      }
      if (currY > 10000) {
        currY = 10000;
      }
      if (currY < -10000) {
        currY = -10000;
      }
      if (currZ > 10000) {
        currZ = 10000;
      }
      if (currZ < -10000) {
        currZ = -10000;
      }
    }
  }
  if (m == 1) {
    if (c.substring(0, 1) == "a") {
      char* buf = (char *)c.c_str();
      int n = sscanf(buf, "a%db%dc%d", &currX, &currY, &currZ);
    }
  }
}
