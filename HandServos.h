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
    String getValue;
    void setupServos();
    void decrement();
    void increment();
    void moveServos(String input);
  private:
    int pinT;
    int pinI;
    int pinM;
    int pinR;

    // Define number of pieces
    const int numberOfPieces = 4;
    String pieces[4];
    String input = "";

    // Keep track of current position in array
    int counter = 0;
    
    // Keep track of the last comma so we know where to start the substring
    int lastIndex = 0;

    int pos;
};

#endif
