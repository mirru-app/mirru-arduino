// this code is meant for an adafruit feather ESP32 board

#include "SPIFFS.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "HandServos.h"
#include "SavePattern.h"
#include "Button2.h"

#define SERVICE_UUID           "0000180d-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "00002a37-b5a3-f393-e0a9-e50e24dcca9e"

#define BUTTON_PIN  12

HandServos handServos;
SavePattern savePattern;
String savedPattern;

bool deviceConnected = false;
BLECharacteristic *pCharacteristic;

int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int startPressed = 0;    // the moment the button was pressed
int endPressed = 0;      // the moment the button was released
int holdTime = 0;        // how long the button was hold
int idleTime = 0;        // how long the button was idle

unsigned long previousMillis = 0; 

Button2 button = Button2(BUTTON_PIN);

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
    }
};

void setup() {
  Serial.begin(115200);
  savePattern.setupSavePattern();
  savedPattern = savePattern.getSavedPattern(SPIFFS, "/savedPattern.txt");
  
  handServos.setupServos();
  
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

  handServos.calibrate();
  button.setClickHandler(handler);
  button.setDoubleClickHandler(handler);
  button.setTripleClickHandler(handler);
  button.setLongClickHandler(handler);
  //button.setTapHandler(tap);
  
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  button.loop();
  buttonReadHold();
}

void handler(Button2& btn) {
    Serial.println(btn.isPressed());
    switch (btn.getClickType()) {
        case SINGLE_CLICK:
            Serial.println("single ");
            break;
        case DOUBLE_CLICK:
            //save pose
            break;
        case TRIPLE_CLICK:
            //go to saved pose
            break;         
    }
}

void saveCurrentPose() {
 // savePattern.readFile(SPIFFS,  "/savedPattern.txt", (char*)rxValueString.c_str());
}

void readMyo() {
  int myo0 = analogRead(A2);
  
  if (myo0 < 750) {
    Serial.println("decrement");
    handServos.closeFingers();
  }
  
  int myo1 = analogRead(A3);
  
  if (myo1 < 500) {
    Serial.println("increment");
    handServos.openFingers();
  }
}

void buttonReadHold() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) { 
     updateState();
  } else {
     updateCounter();
  }
  
  lastButtonState = buttonState;
}

void updateState() {
  if (buttonState == LOW) {
      startPressed = millis();
      idleTime = startPressed - endPressed;
  } else {
      endPressed = millis();
      holdTime = endPressed - startPressed;
  }
}

void updateCounter() {
  if (buttonState == LOW) {
      holdTime = millis() - startPressed;
      if (holdTime >= 1100) {
        handServos.openFingers();
        handServos.openThumb();
      }
  } else {
      idleTime = millis() - endPressed;
  }
}
