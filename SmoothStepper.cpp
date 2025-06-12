#include "SmoothStepper.h"

SmoothStepper::SmoothStepper(uint8_t sPin, uint8_t dPin)
    : stepPin(sPin), dirPin(dPin), currentStep(0), targetSteps(0),
      accel(1000), maxSpeed(1000), currentSpeed(0), running(false) {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void SmoothStepper::move(long steps, float a, float mS) {
    targetSteps = abs(steps);
    digitalWrite(dirPin, steps >= 0 ? HIGH : LOW);
    accel = a;
    maxSpeed = mS;
    currentStep = 0;
    currentSpeed = 0;
    running = true;
    lastStepTime = micros();
    calculateNextStepTime();
}

void SmoothStepper::update() {
    if (!running) return;
    unsigned long now = micros();
    if (now - lastStepTime >= stepInterval) {
        stepMotor();
        lastStepTime = now;
        calculateNextStepTime();
    }
}

void SmoothStepper::stepMotor() {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(stepPin, LOW);
    currentStep++;
    if (currentStep >= targetSteps) {
        running = false;
    }
}

void SmoothStepper::calculateNextStepTime() {
    if (currentStep < targetSteps / 2) {
        currentSpeed += accel * 1e-6;
        if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
    } else {
        currentSpeed -= accel * 1e-6;
        if (currentSpeed < 10) currentSpeed = 10;
    }
    stepInterval = 1e6 / currentSpeed;
}

bool SmoothStepper::isRunning() {
    return running;
}