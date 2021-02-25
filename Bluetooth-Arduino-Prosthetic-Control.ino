// this code is meant for an adafruit feather ESP32 board

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "HandServos.h"
#include "MyoSensors.h"
 
HandServos handServos;
MyoSensors myoSensors;

bool deviceConnected = false;
BLECharacteristic *pCharacteristic;

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
    }
};

void setup(void) {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

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
  
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  myoSensors.readValues();
}
