#ifndef HandServos_h
#define HandServos_h

#include "Arduino.h"
#include <ESP32Servo.h>

class HandServos
{
  public:
    HandServos();
    Servo servoT;
    Servo servoI;
    Servo servoM;
    Servo servoR;
    void setupServos();
    int posT;
    int posI;
    int posM;
    int posR;
    void moveServos(int pos);
  private:
    int pinT;
    int pinI;
    int pinM;
    int pinR;
};

#endif
