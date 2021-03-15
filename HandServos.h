#ifndef HandServos_h
#define HandServos_h

#include "Arduino.h"
#include "Ramp.h"
#include <ESP32Servo.h>

class HandServos
{
  public:
    HandServos(int);
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
    void moveServos2(String input);
    int var;
    int interval;
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
    
    int startPosT = 0;
    int startPosI = 180;
    int startPosM = 180;
    int startPosR = 180;

    int lastPosT = startPosT;
    int lastPosI = startPosI;
    int lastPosM = startPosM;
    int lastPosR = startPosR;
    
    int updateInterval;      // interval between updates
    unsigned long lastUpdate; // last update of position
};

#endif
