#include "Message.h"

Message::Message(String type, int value)
{
    Type = type;
    Value = value;
}

String Message::ToJSON()
{
    String type = String(Type);
    String value = String(Value);
    return "{\"type\":\"" + type + "\",\"value\":\"" + value + "\"}";
}
