#include <Arduino.h>
#include "MPU6050_Handler.h"
#include "RobotActions.h"
#include "BLE_Handler.h"

MPU6050_Handler mpuHandler;
RobotActions robot(&mpuHandler);
BLE_Handler bleHandler(&robot);

void setup() {
    Serial.begin(115200);
    mpuHandler.begin();
    robot.begin();
    bleHandler.begin();
    Serial.println("RoboDog is ready!");
}

void loop() {
    if (robot.getIsMoving()) {
        robot.walk();
    } else {
        robot.stand();
    }
    robot.updateBalance();
    delay(100);
}