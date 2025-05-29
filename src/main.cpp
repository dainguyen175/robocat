#include <Arduino.h>
#include "MPU6050_Handler.h"
#include "RobotActions.h"
#include "BLE_Handler.h"

MPU6050_Handler mpuHandler;
RobotActions robot(&mpuHandler);
BLE_Handler bleHandler(&robot);

void setup() {
  Serial.begin(115200);
  Serial.println("Serial bắt đầu!");

  Serial.println("MPU init...");
  mpuHandler.begin();
  Serial.println("MPU OK");

  Serial.println("Robot init...");
  robot.begin();
  Serial.println("Robot OK");

  Serial.println("BLE init...");
  bleHandler.begin();
  Serial.println("BLE OK");

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