#ifndef MyoSensor_h
#define MyoSensor_h

#include "Arduino.h"

class MyoSensor
{
  public:
    MyoSensor(int pin);
    int sensorValue;
    int analogPin;
    void readValues();
  private:
    long previousMillis = 0;
    long interval = 1000; 
    unsigned long currentMillis;  
};

#endif
