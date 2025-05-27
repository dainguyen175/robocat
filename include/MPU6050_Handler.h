#ifndef MPU6050_HANDLER_H
#define MPU6050_HANDLER_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>

class MPU6050_Handler {
private:
    MPU6050 mpu;
    float pitch, roll;
    float targetPitch = 0;
    float targetRoll = 0;
    float Kp = 2.0;
    float Ki = 0.1;
    float Kd = 0.5;
    float pitchError = 0, rollError = 0;
    float pitchErrorSum = 0, rollErrorSum = 0;
    float lastPitchError = 0, lastRollError = 0;

public:
    MPU6050_Handler();
    void begin();
    void read();
    float getPitch() { return pitch; }
    float getRoll() { return roll; }
    float getPitchAdjustment() { return Kp * pitchError + Ki * pitchErrorSum + Kd * (pitchError - lastPitchError); }
    float getRollAdjustment() { return Kp * rollError + Ki * rollErrorSum + Kd * (rollError - lastRollError); }
};

#endif 