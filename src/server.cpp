// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void initializeSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
}

String replacePlaceholder(const String &var)
{
    Serial.println(var);
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

