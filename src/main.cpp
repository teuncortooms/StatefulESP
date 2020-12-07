/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "config.h"
#include "Led.h"
#include "ServerHelper.h"

ServerHelper(80, "/ws");

// Replace with your network credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// led
Led led(2);

// ldr
const int ldrPin = 36;


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






String readLDR()
{
    return String(analogRead(ldrPin));
}


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
}
