#ifndef ServerHelper_h
#define ServerHelper_h

#include "Arduino.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

class ServerHelper
{
public:
    ServerHelper(uint16_t port);
    void setEndpoints();
    void addHandler(AsyncWebHandler* handler);


private:
    AsyncWebServer server;

    void initializeSPIFFS();
    String replacePlaceholder(const String &var);
};

#endif