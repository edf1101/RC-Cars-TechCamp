/*
This file contains the steering code for 2 paddles to accellerate FWD and REV and gyro to steer.

Note this is the entire file to copy into Driving.cpp
 */


//  --------------------   COPY FROM BELOW HERE -------------------------
#include "src/Driving.h"

#include "src/Radio/Radio.h"
#include "src/FPV/FpvTransmitter.h"


int Driving::gear = 1;

int Driving::calculateThrottle() {
    // This is the steering code for 2 paddles to accellerate FWD and REV

    int throttleMax = FpvTransmitter::throttleMax;


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

    int currentThrottle = 0;

    if (leftIntensity > rightIntensity) {
        // if left paddle is pressed down more than right go Reverse
        currentThrottle = -leftIntensity;
    } else if (rightIntensity > leftIntensity) {
        // if right is pressed down more than left go fwd
        currentThrottle = rightIntensity;
    }
    // if both = 0 neither will be bigger than the other and nothing happens

    return currentThrottle;
}

int Driving::calculateSteering() {
    // This is the default code for working out the steering. Just based on the tilt values

    return FpvTransmitter::rawTilt;
}
