// this code is meant for an adafruit feather ESP32 board

#include "SPIFFS.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "HandServos.h"
#include "SavePattern.h"

HandServos handServos;
SavePattern savePattern;
String savedPattern;

bool deviceConnected = false;
BLECharacteristic *pCharacteristic;
bool isToggled;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

#define SERVICE_UUID           "0000180d-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "00002a37-b5a3-f393-e0a9-e50e24dcca9e"

class OnConnectCallback: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Disconnected");
    }
};

class ReceivedDataCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      String rxValueString = pCharacteristic->getValue().c_str();
      handServos.moveServos(rxValueString);

      int str_len = rxValueString.length() + 1; 
      char * char_array[str_len];
      String theString = rxValueString;
      
      savePattern.writeFile(SPIFFS,  "/savedPattern.txt", (char*)rxValueString.c_str());
      //write array of strings to record movement
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  savePattern.setupSavePattern();

  savedPattern = savePattern.getSavedPattern(SPIFFS, "/savedPattern.txt");
  
  handServos.setupServos();
  //handServos.calibrate();
  handServos.moveServos(savedPattern);

  BLEDevice::init("Brunel Hand");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new OnConnectCallback());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new ReceivedDataCallback());
  pService->start();
  pServer->getAdvertising()->start();
  
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  toggleButton();

  if (isToggled) {
    Serial.println("SDFSFD");
    handServos.increment();
  }
}

void toggleButton() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(12);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        isToggled = !isToggled;
        Serial.println(isToggled);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void readMyo() {
  int myo0 = analogRead(A2);
  
  if (myo0 < 750) {
    Serial.println("decrement");
    handServos.decrement();
    // goes from 180upright degrees to 0bent degrees
  }
  
  int myo1 = analogRead(A3);
  
  if (myo1 < 500) {
    Serial.println("increment");
    handServos.increment();
    // goes from 0 bent degrees to 180upright degrees
  }
}
