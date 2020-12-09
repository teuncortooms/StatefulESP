#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "config.h"
#include "Led.h"
#include "Message.h"

// Create server and websocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Set credentials in config.h
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// led and ldr
Led led(2);
const int ldrPin = 36;

String readLDR();

// wifi
void connectToWifi();

// websocket
void initWebSocket();
void notifyClientsOfLedState();
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);

// http controller
void initializeSPIFFS();
String replacePlaceholder(const String &var);
void setEndpoints();

void setup()
{
    Serial.begin(115200);
    pinMode(ldrPin, INPUT);

    connectToWifi();
    initWebSocket();
    initializeSPIFFS();
    setEndpoints();

    server.begin();
}

void loop()
{
    ws.cleanupClients();
    led.Update();

    if (millis() - previousMillis > timeBetweenReading)
    {
        sendLdrReading();
        previousMillis = millis();
    }
}

void connectToWifi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP());
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                      client->remoteIP().toString().c_str());
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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        if (strcmp((char *)data, "TOGGLE_LED") == 0)
        {
            led.ToggleState();
            notifyClientsOfLedState();
        }
    }
}

void notifyClientsOfLedState()
{
    Message msg(LEDSTATE, led.GetState());
    ws.textAll(msg.ToJSON());
}

void initializeSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
}

void setEndpoints()
{
    // root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false, replacePlaceholder);
    });
    // css
    server.on("/css/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/index.css", "text/css");
    });
    // js
    server.on("/js/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/index.js", "application/javascript");
    });
    server.on("/js/control.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/control.js", "application/javascript");
    });
    server.on("/js/sensors.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/sensors.js", "application/javascript");
    });
    // sensor endpoints
    server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/plain", readLDR().c_str());
    });
}

String replacePlaceholder(const String &var)
{
    if (var == "STATE")
    {
        if (led.GetState())
        {
            return "ON";
        }
        else
        {
            return "OFF";
        }
    }
    return "ERROR";
}

String readLDR()
{
    return String(analogRead(ldrPin));
}

void sendLdrReading()
{
    int value = analogRead(ldrPin);
    Message msg(LDRVALUE, value);
    ws.textAll(msg.ToJSON());
}