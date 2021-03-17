#include "Arduino.h"
#include "HandServo.h"

HandServo::HandServo(int interval)
{
  updateInterval = interval;
  increment = 1;
}

void HandServo::setupServo(int pin) {
  servo.attach(pin);
}

void HandServo::moveServo() {  
  if((millis() - lastUpdate) > updateInterval)  // time to update
  {
    lastUpdate = millis();
    pos += increment;
    servo.write(pos);

    if ((pos >= 180) || (pos <= 0)) // end of sweep
    {
      // reverse direction
      increment = -increment;
    }
  }

//  if((millis() - lastUpdate) > updateInterval) {
//    lastUpdate = millis();
//    Serial.println("move");
//    if (lastPos > newPos) {
//       for (int i = lastPos; i > newPos; i--) {
//          servo.write(i);
//          lastPos = servo.read();
//          delay(15);
//       }
//      } else if (lastPos < newPos) {
//        for (int i = lastPos; i < newPos; i++) {
//          servo.write(i);
//          lastPos = servo.read();
//          delay(15);
//        }
//      }
//  }
}

void HandServo::calibrate() {
  int pos = lastPos;
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
      servo.write(pos);
      delay(15);             // waits 15ms for the servo to reach the position
  }
  lastPos = servo.read();
}
