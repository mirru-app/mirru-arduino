#include "Arduino.h"
#include "MyoSensor.h"

MyoSensor::MyoSensor(int pin)
{
  analogPin = pin;
  sensorValue = analogRead(analogPin);
}

void MyoSensor::readValues() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.println(sensorValue);
    delay(1);
  }
}
