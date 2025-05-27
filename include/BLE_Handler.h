#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "RobotActions.h"

class BLE_Handler {
private:
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    bool deviceConnected;
    RobotActions* robot;
    static const char* SERVICE_UUID;
    static const char* CHARACTERISTIC_UUID;

public:
    BLE_Handler(RobotActions* robotActions);
    void begin();
    void update();
    bool isConnected() { return deviceConnected; }
};

#endif 