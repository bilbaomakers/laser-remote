#include "WifiManager.h"

WifiManager::WifiManager(const char* ssid, const char* password) : _server(80), _ws("/ws") {
  // Contstructor
  _ssid = ssid;
  _password = password;
}

void WifiManager::setup(std::function<void(char* speedX, char* speedY)> setSpeed) {
  // setup
  _setSpeed = setSpeed;
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(_ssid, _password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  _ws.onEvent([&](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    onEvent(server, client, type, arg, data, len);
  });
  _server.addHandler(&_ws);

  // Route for root / web page
  _server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    Serial.println("Load /");
    request->send(200, "text/html", (String)_index_html);
  });

  // Start server
  _server.begin();
}

void WifiManager::loop() {
  // loop
  _ws.cleanupClients();
}

void WifiManager::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    char* chars_array = strtok((char*)data, "|:");
    char* xy[2] = {0, 0};
    int index = 0;
    while(chars_array)
    {
        if (chars_array[0] == 'x') {
            index = 0;
        } else if (chars_array[0] == 'y') {
            index = 1;
        } else {
            xy[index] = chars_array;
        }
        chars_array = strtok(NULL, "|:");
    }
    _setSpeed(xy[0], xy[1]);
  }
}

void WifiManager::onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}