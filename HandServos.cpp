#include "Arduino.h"
#include "HandServos.h"

HandServos::HandServos()
{ 
  posT = 0;
  posI = 0;
  posM = 0;
  posR = 0;
  
  pinT = 32;
  pinI = 33;
  pinM = 14;
  pinR = 15;
}

void HandServos::setupServos() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  servoT.setPeriodHertz(50);
  servoI.setPeriodHertz(50);
  servoM.setPeriodHertz(50);
  servoR.setPeriodHertz(50);
  
  servoT.attach(pinT);
  servoI.attach(pinI);
  servoM.attach(pinM);
  servoR.attach(pinR);
}

void HandServos::moveServos(int posT)
{  
    servoT.write(posT);
    servoI.write(posT);
    servoM.write(posT);
    servoR.write(posT);
    delay(15);

}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
