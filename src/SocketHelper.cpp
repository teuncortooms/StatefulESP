#include "SocketHelper.h"

SocketHelper::SocketHelper(const String& url)
: ws(url)
{

}

void SocketHelper::notifyClients()
{
    ws.textAll(String(led.GetState()));
}

void SocketHelper::handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        if (strcmp((char *)data, "toggle") == 0)
        {
            led.ToggleState();
            notifyClients();
        }
    }
}

void SocketHelper::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
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

void SocketHelper::initWebSocket(AsyncWebServer* server)
{
    ws.onEvent(SocketHelper::onEvent);
    server.addHandler(&ws);
}