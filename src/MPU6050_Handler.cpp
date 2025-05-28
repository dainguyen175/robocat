#include "MPU6050_Handler.h"

MPU6050_Handler::MPU6050_Handler() {
}

void MPU6050_Handler::begin() {
    Wire.begin();
    mpu.initialize();
    
    // Kiểm tra kết nối
    if (!mpu.testConnection()) {
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        while(1) delay(500);
    }
    
    // Cấu hình MPU6050
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
}

void MPU6050_Handler::read() {
    // Đọc dữ liệu từ MPU6050
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Chuyển đổi dữ liệu thành góc
    float accelX = ax / 16384.0;
    float accelY = ay / 16384.0;
    float accelZ = az / 16384.0;
    
    pitch = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * RAD_TO_DEG;
    roll = atan2(-accelX, accelZ) * RAD_TO_DEG;
    
    pitchError = targetPitch - pitch;
    rollError = targetRoll - roll;
    
    pitchErrorSum += pitchError;
    rollErrorSum += rollError;
    
    lastPitchError = pitchError;
    lastRollError = rollError;
} 