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
#define LED_PIN A0

HandServos handServos(15);
SavePattern savePattern;
String savedPattern;
String rxValueString;
bool freezeThumb = false;

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
        rxValueString = pCharacteristic->getValue().c_str();
        handServos.moveServos(rxValueString, freezeThumb);
  
        int str_len = rxValueString.length() + 1; 
        char * char_array[str_len];
        savePattern.setLastPattern(rxValueString);
        //(char*)rxValueString.c_str()
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  savePattern.setupSavePattern();
  savedPattern = savePattern.readFileString(SPIFFS, "/savedPattern.txt");
  Serial.println(savedPattern);
  
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
  //https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLETests/Arduino/BLE_uart/BLE_uart.ino
  pServer->getAdvertising()->addServiceUUID(pService->getUUID());
  pServer->getAdvertising()->start();

  handServos.calibrate();
  button.setClickHandler(handler);
  button.setDoubleClickHandler(handler);
  button.setTripleClickHandler(handler);
  button.setDoubleClickTime(500);
  button.setLongClickTime(2000);
  button.setLongClickDetectedHandler(longClick);

  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  button.loop();
  delay(15);
  buttonReadHold();
  delay(15);
}

void handler(Button2& btn) {
    switch (btn.getClickType()) {
      case SINGLE_CLICK:
          freezeThumbButton();
          break;
      case DOUBLE_CLICK:
          load();
          break;
      case TRIPLE_CLICK:
          save();
          break;
    }
}

void freezeThumbButton() {
  Serial.println("single");
  freezeThumb = !freezeThumb;
  if (freezeThumb) {
      Serial.println("on");
      digitalWrite(LED_PIN, 1);
      delay(5);
  } else {
      digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
      Serial.println("off");
      delay(5);  
  }
}

void load() {
  Serial.println("double load");
  savedPattern = savePattern.readFileString(SPIFFS,  "/savedPattern.txt");
  handServos.moveServos2(savedPattern);
  Serial.println(savedPattern);
}

void save() {
  savePattern.writeFile(SPIFFS,  "/savedPattern.txt", (char*)savePattern.lastPattern.c_str());
  Serial.println("triple save");
  savedPattern = savePattern.readFileString(SPIFFS,  "/savedPattern.txt");
  Serial.println(savedPattern);
}

void longClick(Button2& btn) {
  Serial.println("long press");
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
      }
  } else {
      idleTime = millis() - endPressed;
  }
}
