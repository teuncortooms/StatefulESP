#ifndef SocketHelper_h
#define SocketHelper_h

#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class SocketHelper
{
public:
    SocketHelper(const String& url);
    void initWebSocket(AsyncWebServer* server);
    void notifyClients();

private:
    AsyncWebSocket ws;

    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len);
};

#endif