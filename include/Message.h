#ifndef Message_h
#define Message_h

#include "Arduino.h"

// enum MessageType
// {
//     LEDSTATE,
//     LDRVALUE
// };

class Message
{
public:
    Message(String type, int value);
    String ToJSON();

private:
    String Type;
    int Value;
};

#endif