#ifndef MyoSensor_h
#define MyoSensor_h

#include "Arduino.h"

class MyoSensors
{
  public:
    MyoSensors();
    int sensorValue;
    void readValues();
  private:
    int a;
};

#endif
