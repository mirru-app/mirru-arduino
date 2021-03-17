#ifndef HandServo_h
#define HandServo_h

#include "Arduino.h"
#include <ESP32Servo.h>

class HandServo
{
  public:
    HandServo(int);
    Servo servo;
    void setupServo(int pin);
    void moveServo();
    void calibrate();
    int updateInterval;      // interval between updates
    int increment = 1;        // increment to move for each interval    

  private:
    int pin;
    unsigned long lastUpdate; // last update of position
    int pos;              // current servo position
    int interval;
    int lastPos = 0;
};

#endif
