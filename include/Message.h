#ifndef Message_h
#define Message_h

#include "Arduino.h"

enum MessageType
{
    LEDSTATE,
    LDRVALUE
};

class Message
{

public:
    Message(MessageType type, int value);
    String ToJSON();

private:
    MessageType Type;
    int Value;
};

#endif