#include "BLE_Handler.h"

const char* BLE_Handler::SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* BLE_Handler::CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
        // Restart advertising
        BLEDevice::startAdvertising();
        Serial.println("Restart advertising");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    RobotActions* robot;
public:
    MyCallbacks(RobotActions* r) : robot(r) {}
    
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value == "START") {
            if (!robot->getIsSitting()) {
                robot->setIsMoving(true);
                Serial.println("Received START command");
            }
        } else if (value == "STOP") {
            robot->stop();
            Serial.println("Received STOP command");
        } else if (value == "SIT") {
            robot->sit();
            Serial.println("Received SIT command");
        }
    }
};

BLE_Handler::BLE_Handler(RobotActions* robotActions) : robot(robotActions), deviceConnected(false) {
}

void BLE_Handler::begin() {
    BLEDevice::init("RoboDog");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks(robot));
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
}

void BLE_Handler::update() {
    // Add any BLE update logic here if needed
} 