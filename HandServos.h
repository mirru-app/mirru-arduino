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
    void calibrate();
    void openFingers();
    void openThumb();
    void closeFingers();
    void moveServos(String input);
    void movee(int var);
    int var;
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

    int startPos = 180;
    int startPosThumb = 0;
    int lastPos = startPos;
    int lastPosThumb = startPosThumb;
    int posI;
    int posM;
    int posR;
    int i;
};

#endif
