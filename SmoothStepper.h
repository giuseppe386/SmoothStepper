#ifndef SMOOTHSTEPPER_H
#define SMOOTHSTEPPER_H

#include <Arduino.h>

class SmoothStepper {
public:
    SmoothStepper(uint8_t stepPin, uint8_t dirPin);

    void move(long steps, float accel = 1000.0, float maxSpeed = 1000.0);
    void update(); // da chiamare in loop()
    bool isRunning();

private:
    void stepMotor();
    void calculateNextStepTime();

    uint8_t stepPin, dirPin;
    long currentStep, targetSteps;
    float accel, maxSpeed;
    float currentSpeed, stepInterval;
    unsigned long lastStepTime;
    bool running;
};

#endif