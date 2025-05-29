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
    if (!isSitting && isMoving) {
        // Tăng tốc độ di chuyển bằng cách giảm delay
        static int step = 0;
        const int WALK_STEPS = 4;  // Số bước trong một chu kỳ đi bộ
        
        switch(step) {
            case 0:
                // Bước 1: Nâng chân trước bên trái và sau bên phải
                servos[0].write(WALKING_POSITIONS[0] + 30);  // Tăng góc lên
                servos[5].write(WALKING_POSITIONS[5] + 30);
                servos[1].write(WALKING_POSITIONS[1]);
                servos[2].write(WALKING_POSITIONS[2]);
                servos[3].write(WALKING_POSITIONS[3]);
                servos[4].write(WALKING_POSITIONS[4]);
                break;
            case 1:
                // Bước 2: Đẩy chân trước bên trái và sau bên phải về sau
                servos[0].write(WALKING_POSITIONS[0] - 30);  // Tăng góc xuống
                servos[5].write(WALKING_POSITIONS[5] - 30);
                servos[1].write(WALKING_POSITIONS[1]);
                servos[2].write(WALKING_POSITIONS[2]);
                servos[3].write(WALKING_POSITIONS[3]);
                servos[4].write(WALKING_POSITIONS[4]);
                break;
            case 2:
                // Bước 3: Nâng chân trước bên phải và sau bên trái
                servos[3].write(WALKING_POSITIONS[3] + 30);
                servos[2].write(WALKING_POSITIONS[2] + 30);
                servos[0].write(WALKING_POSITIONS[0]);
                servos[1].write(WALKING_POSITIONS[1]);
                servos[4].write(WALKING_POSITIONS[4]);
                servos[5].write(WALKING_POSITIONS[5]);
                break;
            case 3:
                // Bước 4: Đẩy chân trước bên phải và sau bên trái về sau
                servos[3].write(WALKING_POSITIONS[3] - 30);
                servos[2].write(WALKING_POSITIONS[2] - 30);
                servos[0].write(WALKING_POSITIONS[0]);
                servos[1].write(WALKING_POSITIONS[1]);
                servos[4].write(WALKING_POSITIONS[4]);
                servos[5].write(WALKING_POSITIONS[5]);
                break;
        }
        
        step = (step + 1) % WALK_STEPS;
        delay(100);  // Giảm delay để tăng tốc độ
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