#ifndef ROBOT_ACTIONS_H
#define ROBOT_ACTIONS_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "MPU6050_Handler.h"

class RobotActions {
private:
    Servo servos[6];
    const int servoPins[6] = {13, 12, 14, 27, 26, 25};
    MPU6050_Handler* mpuHandler;
    bool isMoving = false;
    bool isSitting = false;

public:
    const int STANDING_POSITIONS[6] = {90, 90, 90, 90, 90, 90};
    const int WALKING_POSITIONS[6] = {60, 120, 60, 120, 60, 120};
    const int SITTING_POSITIONS[6] = {45, 45, 135, 135, 45, 45}; // Góc servo khi ngồi

    RobotActions(MPU6050_Handler* mpu);
    void begin();
    void moveToPosition(const int* positions);
    void stand();
    void walk();
    void sit();
    void stop();
    void updateBalance();
    bool getIsMoving() { return isMoving; }
    void setIsMoving(bool state) { isMoving = state; }
    bool getIsSitting() { return isSitting; }
    void setIsSitting(bool state) { isSitting = state; }
};

#endif 