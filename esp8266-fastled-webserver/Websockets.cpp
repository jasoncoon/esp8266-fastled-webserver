/*
   ESP8266 FastLED WebServer: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if ENABLE_WEBSOCKETS

#include "common.h"

// #include <WebSockets.h>
#include <WebSocketsServer.h>


WebSocketsServer webSocketsServer = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;

        case WStype_CONNECTED:
            IPAddress ip = webSocketsServer.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            //send message to client
            webSocketsServer.sendTXT(num, "Connected");
            break;

        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            //send message to client
            webSocketsServer.sendTXT(num, payload);

            //send data to all connected clients
            webSocketsServer.broadcastTXT(payload);
            
            break;

        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            //send message to client
            webSocketsServer.sendBIN(num, payload, length);
            break;
    }
}

void InitializeWebSocketServer(void) {
    webSocketsServer.begin();
    webSocketsServer.onEvent(webSocketEvent);
    Serial.println("Web socket server started");
}
void broadcastInt(String name, uint8_t value) {
    String json = "{\"name\":\"" + name + "\",\"value\":" + String(value) + "}";
    webSocketsServer.broadcastTXT(json);
}
void broadcastString(String name, String value)
{
    String json = "{\"name\":\"" + name + "\",\"value\":\"" + String(value) + "\"}";
    webSocketsServer.broadcastTXT(json);
}


#endif // ENABLE_WEBSOCKETS
