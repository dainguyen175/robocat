#include "MPU6050_Handler.h"

MPU6050_Handler::MPU6050_Handler() {
}

void MPU6050_Handler::begin() {
    Wire.begin();
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }
    
    mpu.calibrateGyro();
    mpu.setThreshold(3);
}

void MPU6050_Handler::read() {
    Vector normGyro = mpu.readNormalizeGyro();
    Vector normAccel = mpu.readNormalizeAccel();
    
    pitch = atan2(normAccel.YAxis, sqrt(normAccel.XAxis * normAccel.XAxis + normAccel.ZAxis * normAccel.ZAxis)) * RAD_TO_DEG;
    roll = atan2(-normAccel.XAxis, normAccel.ZAxis) * RAD_TO_DEG;
    
    pitchError = targetPitch - pitch;
    rollError = targetRoll - roll;
    
    pitchErrorSum += pitchError;
    rollErrorSum += rollError;
    
    lastPitchError = pitchError;
    lastRollError = rollError;
} 