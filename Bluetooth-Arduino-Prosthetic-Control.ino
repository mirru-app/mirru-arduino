#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "HandServos.h"

HandServos handServos;
bool deviceConnected = false;
BLECharacteristic *pCharacteristic;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      
      String rxValueString = pCharacteristic->getValue().c_str();
      Serial.println(rxValueString);

      handServos.moveServos();

      if (rxValueString == "on") { 
        Serial.println("Turning on");
        digitalWrite(13, HIGH);
      }
        
      if (rxValueString == "off") {
        Serial.println("Turning off");
        digitalWrite(13, LOW);
      }
      
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.println(rxValue[i]);
        }
      }
    }
};

void setup(void) {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  handServos.setupServos();
  BLEDevice::init("ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
  
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
//loop here
}
