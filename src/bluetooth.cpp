#include "bluetooth.h"
#include <Arduino.h>

BLECharacteristic* pCharacteristic;
bool deviceConnected = false;

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected, restarting system...");
    delay(100);  // Give time for Serial to flush
    ESP.restart();
}

void bluetooth_setup() {
    BLEDevice::init("ESP32-BVT");
    BLEServer* pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService* pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
    );

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("0");
    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("Waiting for bluetooth connection...");
}