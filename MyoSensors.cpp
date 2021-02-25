#include "Arduino.h"
#include "MyoSensors.h"

MyoSensors::MyoSensors()
{ 
  sensorValue = analogRead(A0);
}

void MyoSensors::readValues() {
  // read the input on analog pin 0:
  sensorValue = analogRead(A0);
  // print out the value you read:
   Serial.println(sensorValue);
   // delay in between reads for stability
  delay(1);
}
