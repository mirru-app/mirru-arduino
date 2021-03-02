#include "Arduino.h"
#include "HandServos.h"

/*
T = thumb
I = index
M = middle
R = ring
*/

HandServos::HandServos()
{ 
  pinT = 27;
  pinI = 32;
  pinM = 15;
  pinR = 14;
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

void HandServos::decrement() {
  pos = servoT.read();
  for (int i = pos; i >= 0; i -= 1) { // goes from 180 degrees to 0 degrees
    servoT.write(i);
    servoI.write(i);
    servoM.write(i);
    servoR.write(i); // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
}

void HandServos::increment() {
  pos = servoT.read();
  for (int i = pos; i <= 180; i += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoT.write(i);
    servoI.write(i);
    servoM.write(i);
    servoR.write(i);
    delay(15);             // waits 15ms for the servo to reach the position
  }
}

void HandServos::moveServos(String input)
{ 
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
    delay(15);
}
