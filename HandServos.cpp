#include "Arduino.h"
#include "HandServos.h"

/*
T = thumb
I = index
M = middle
R = ring
*/

HandServos::HandServos(int interval)
{
  pinT = 27;
  pinI = 32;
  pinM = 15;
  pinR = 14;
  
  updateInterval = interval;
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

void HandServos::closeFingers() {
  if (lastPosI > 0) {
    int maxPos = lastPosI-2;
    for (int i = lastPosI; i >= maxPos; i -= 1) { // goes from 180 degrees to 0bent degrees
      // in steps of 1 degree
      Serial.println(i);
      servoT.write(i);
      servoI.write(i);
      servoM.write(i);
      servoR.write(i);
      lastPosI = i;
      delay(15);             // waits 15ms for the servo to reach the position
      
    }
  } else {
    lastPosI = 180;
  }
  lastPosI = servoI.read();
}

void HandServos::openThumb() {
  if (lastPosT > 0) {
    int maxPos = lastPosT-20;
    for (int i = lastPosT; i >= maxPos; i -= 1) {
      Serial.println(i);
      servoT.write(i);
      lastPosT = i;
      delay(15);
    }
  } else {
    lastPosT = 180;
  }
  lastPosT = servoT.read();
}

void HandServos::openFingers() {
  if (lastPosI < 180) {
    for (int i = lastPosI; i <= 180; i += 1) {
      // in steps of 1 degree
      Serial.println(i);
      servoI.write(i);
      lastPosI = i;
      Serial.println(i);
      delay(15);
    }
  } else {
    lastPosI = 0;
  }

  if (lastPosM < 180) {
    for (int i = lastPosM; i <= 180; i += 1) {
      // in steps of 1 degree
      Serial.println(i);
      servoM.write(i);
      lastPosM = i;
      Serial.println(i);
      delay(15);
    }
  } else {
    lastPosM = 0;
  }

  if (lastPosR < 180) {
    for (int i = lastPosR; i <= 180; i += 1) {
      // in steps of 1 degree
      Serial.println(i);
      servoR.write(i);
      lastPosR = i;
      Serial.println(i);
      delay(15);
    }
  } else {
    lastPosR = 0;
  }
  lastPosI = servoI.read();
  lastPosM = servoM.read();
  lastPosR = servoR.read();
}

void HandServos::calibrate() {
  int pos = startPosI;
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
      servoT.write(pos);
      servoI.write(pos);
      servoM.write(pos);
      servoR.write(pos);
      delay(15);             // waits 15ms for the servo to reach the position
  }
  lastPosT = servoT.read();
  lastPosI = servoI.read();
  lastPosM = servoM.read();
  lastPosR = servoR.read();
}

void HandServos::moveServos(String input) {
  //parse String
  //https://gist.github.com/mattfelsen/9467420
  
  for (int i = 0; i < input.length(); i++) {
    // Loop through each character and check if it's a comma
    if (input.substring(i, i+1) == ",") {
      // Grab the piece from the last index up to the current position and store it
      pieces[counter] = input.substring(lastIndex, i);
      // Update the last position and add 1, so it starts from the next character
      lastIndex = i + 1;
      // Increase the position in the array that we store into
      counter++;
    }

    // If we're at the end of the string (no more commas to stop us)
    if (i == input.length() - 1) {
      // Grab the last part of the string from the lastIndex to the end
      pieces[counter] = input.substring(lastIndex, i+1);

      //reset for next incoming string
      input = "";
      counter = 0;
      lastIndex = 0;
    }
  }

//    debug
    Serial.println("pieces");
    Serial.println(pieces[0]);
    Serial.println(pieces[1]);
    Serial.println(pieces[2]);
    Serial.println(pieces[3]);

    servoT.write(pieces[0].toInt());
    servoI.write(pieces[1].toInt());
    servoM.write(pieces[2].toInt());
    servoR.write(pieces[3].toInt());
    
    lastPosT = servoT.read();
    lastPosI = servoI.read();
    lastPosM = servoM.read();
    lastPosR = servoR.read();
    delay(15);
}

void HandServos::moveServos2(String input) {
  //parse String
  //https://gist.github.com/mattfelsen/9467420
  
  for (int i = 0; i < input.length(); i++) {
    // Loop through each character and check if it's a comma
    if (input.substring(i, i+1) == ",") {
      // Grab the piece from the last index up to the current position and store it
      pieces[counter] = input.substring(lastIndex, i);
      // Update the last position and add 1, so it starts from the next character
      lastIndex = i + 1;
      // Increase the position in the array that we store into
      counter++;
    }

    // If we're at the end of the string (no more commas to stop us)
    if (i == input.length() - 1) {
      // Grab the last part of the string from the lastIndex to the end
      pieces[counter] = input.substring(lastIndex, i+1);

      //reset for next incoming string
      input = "";
      counter = 0;
      lastIndex = 0;
    }
  }

    Serial.println("pieces");
    Serial.println(pieces[0]);
    Serial.println(pieces[1]);
    Serial.println(pieces[2]);
    Serial.println(pieces[3]);

    if (lastPosT < pieces[0].toInt()) {
      for (int i = lastPosT; i < pieces[0].toInt(); i++) {
        servoT.write(i);
        lastPosT = servoT.read();
        delay(15);
      }
    }
    
    // I
    if (lastPosI > pieces[1].toInt()) {
     for (int i = lastPosI; i > pieces[1].toInt(); i--) {
        servoI.write(i);
        lastPosI = servoI.read();
        delay(15);
     }
    } else if (lastPosI < pieces[1].toInt()) {
      for (int i = lastPosI; i < pieces[1].toInt(); i++) {
        servoI.write(i);
        lastPosI = servoI.read();
        delay(15);
      }
    }

    // M
    if (lastPosM > pieces[2].toInt()) {
     for (int i = lastPosM; i > pieces[2].toInt(); i--) {
        servoM.write(i);
        lastPosM = servoM.read();
        delay(15);
     }
    } else if (lastPosM < pieces[2].toInt()) {
      for (int i = lastPosM; i < pieces[2].toInt(); i++) {
        servoM.write(i);
        lastPosM = servoM.read();
        delay(15);
      }
    }

    // R
    if (lastPosR > pieces[3].toInt()) {
     for (int i = lastPosR; i > pieces[3].toInt(); i--) {
        servoR.write(i);
        lastPosR = servoR.read();
        delay(15);
     }
    } else if (lastPosR < pieces[3].toInt()) {
      for (int i = lastPosR; i < pieces[3].toInt(); i++) {
        servoR.write(i);
        lastPosR = servoR.read();
        delay(15);
      }
    }

    if (lastPosT > pieces[0].toInt()) {
     for (int i = lastPosT; i > pieces[0].toInt(); i--) {
        servoT.write(i);
        lastPosT = servoT.read();
        delay(15);
     }
    } 
}
