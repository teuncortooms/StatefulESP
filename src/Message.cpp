#include "Message.h"

Message::Message(String type, int value)
{
    type = type;
    value = value;
}

String Message::ToJSON()
{
    String t = String(t);
    String v = String(v);
    return "{\"type\":\"" + t + "\",\"value\":\"" + v + "\"}";
}
