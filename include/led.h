#ifndef led_h
#define led_h

#include "Arduino.h"

class Led
{
public:
    Led(int pin);
    void ToggleState();
    bool GetState();
    void Update();

private:
    bool ledState = 0;
    int ledPin;
};

#endif