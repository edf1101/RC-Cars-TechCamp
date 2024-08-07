#include "src/Driving.h"
#include "src/Radio/Radio.h"
#include "src/FPV/FpvTransmitter.h"

int Driving::gear = 1;

void Driving::setGear(int _gear) {
    gear = _gear;
    if (gear < 1)
        gear = 1;
    if (gear > totalGears)
        gear = totalGears;
}

int Driving::getGear() {
    return gear;
}

int Driving::getMaxGear() {
    return totalGears;
}


int Driving::calculateThrottle() {
    // This is the default code for working out throttle with the
    // paddle switches for FWD / REV, and paddle for intensity

    int throttleMax = (float) FpvTransmitter::throttleMax * (float(gear) / float(totalGears));
     targetThrottle = 0;

    if (!digitalRead(FpvTransmitter::swA) && digitalRead(FpvTransmitter::swB)) {
        // brake pressed and not accellerator
        targetThrottle = -throttleMax;
    }
    if (digitalRead(FpvTransmitter::swA) && !digitalRead(FpvTransmitter::swB)) {
        // accellerator pressed and not brake
        targetThrottle = throttleMax;
    }
    float deltaTime = float(millis()-lastMillis)/1000.0;
    lastMillis= millis();

    return lerp(throttle,targetThrottle,0.2);
}


int Driving::calculateSteering() {
    // This is the default code for working out the steering. Just based on the tilt values


    // get the paddle values
    float leftIntensity = analogRead(FpvTransmitter::mag1);
    int leftRange = getPaddleLeftLow() - getPaddleLeftHigh();
    leftIntensity = leftIntensity - getPaddleLeftHigh();
    leftIntensity = 100 - (100 * leftIntensity / leftRange);
    leftIntensity = (leftIntensity < 30) ? 0 : leftIntensity;
    leftIntensity = map(float(leftIntensity), 30, 100, 0, 100);
    leftIntensity = min(100.0f, max(0.0f, leftIntensity));

    float rightIntensity = analogRead(FpvTransmitter::mag2);
    int rightRange = getPaddleRightLow() - getPaddleRightHigh();
    rightIntensity = rightIntensity - getPaddleRightHigh();
    rightIntensity = 100 - (100 * rightIntensity / rightRange);
    rightIntensity = (rightIntensity < 30) ? 0 : rightIntensity;
    rightIntensity = map(float(rightIntensity), 30, 100, 0, 100);
    rightIntensity = min(100.0f, max(0.0f, rightIntensity));

    int tempSteering = (rightIntensity > leftIntensity) ? rightIntensity : -leftIntensity;

    // needs to be returned as a 0-180 servo angle
    return map(tempSteering, -100, 100, 0, 180);
}
