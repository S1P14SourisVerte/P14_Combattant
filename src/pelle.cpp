  
void vidage(void){
    unsigned long startTime = millis(); 
    BoardInit();
    SERVO_Enable(0);

    while (millis()-startTime < 3000){
        MOTOR_SetSpeed(0, 0.3);
        MOTOR_SetSpeed(1, 0.3);
    }
    SERVO_SetAngle(0, 0);
    delay(1000);
    SERVO_SetAngle(0, 90);
    delay(500);
    SERVO_SetAngle(0, 120);
    delay(300);
    SERVO_SetAngle(0, 90);
    delay(300);
    SERVO_SetAngle(0, 120);
    delay(300);
    SERVO_SetAngle(0, 90);
    delay(300);
    SERVO_SetAngle(0, 120);
    delay(300);
    SERVO_SetAngle(0, 0);
    delay(2000);
    }
