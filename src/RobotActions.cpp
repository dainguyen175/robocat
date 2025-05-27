#include "RobotActions.h"

RobotActions::RobotActions(MPU6050_Handler* mpu) : mpuHandler(mpu) {
}

void RobotActions::begin() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    for (int i = 0; i < 6; i++) {
        servos[i].setPeriodHertz(50);
        servos[i].attach(servoPins[i], 500, 2400);
        servos[i].write(STANDING_POSITIONS[i]);
    }
}

void RobotActions::moveToPosition(const int* positions) {
    for (int i = 0; i < 6; i++) {
        servos[i].write(positions[i]);
    }
}

void RobotActions::stand() {
    isSitting = false;
    moveToPosition(STANDING_POSITIONS);
}

void RobotActions::walk() {
    if (!isSitting) {
        moveToPosition(WALKING_POSITIONS);
    }
}

void RobotActions::sit() {
    isSitting = true;
    isMoving = false;
    
    // Di chuyển từ từ đến vị trí ngồi
    for (int i = 0; i < 6; i++) {
        int currentPos = servos[i].read();
        int targetPos = SITTING_POSITIONS[i];
        
        // Di chuyển từng bước nhỏ
        while (abs(currentPos - targetPos) > 5) {
            if (currentPos < targetPos) {
                currentPos += 5;
            } else {
                currentPos -= 5;
            }
            servos[i].write(currentPos);
            delay(50); // Đợi servo di chuyển
        }
        servos[i].write(targetPos);
    }
}

void RobotActions::stop() {
    isMoving = false;
    if (!isSitting) {
        stand();
    }
}

void RobotActions::updateBalance() {
    if (!isSitting) {  // Chỉ giữ thăng bằng khi không ngồi
        mpuHandler->read();
        float pitchAdjustment = mpuHandler->getPitchAdjustment();
        float rollAdjustment = mpuHandler->getRollAdjustment();
        
        for(int i = 0; i < 6; i++) {
            int currentPos = servos[i].read();
            int newPos = currentPos;
            
            if(i < 3) {
                newPos += pitchAdjustment;
            } else {
                newPos -= pitchAdjustment;
            }
            
            if(i % 2 == 0) {
                newPos += rollAdjustment;
            } else {
                newPos -= rollAdjustment;
            }
            
            newPos = constrain(newPos, 0, 180);
            servos[i].write(newPos);
        }
    }
} 