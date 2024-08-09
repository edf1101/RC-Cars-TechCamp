#ifndef DRIVING_H
#define DRIVING_H

#include "Arduino.h"

class Driving {
public:
    void calculateDriving() {
        throttle = calculateThrottle();
        steering = calculateSteering();
    }

    int8_t getThrottle() const { return throttle; }
    int8_t getSteering() const { return steering; }

    void setTriggerLeftRange(int low, int high) {
        paddleLeftHigh = high;
        paddleLeftLow = low;
    }

    void setTriggerRightRange(int low, int high) {
        paddleRightHigh = high;
        paddleRightLow = low;
    }

    int getPaddleLeftLow() const { return paddleLeftLow; }
    int getPaddleLeftHigh() const { return paddleLeftHigh; }
    int getPaddleRightLow() const { return paddleRightLow; }
    int getPaddleRightHigh() const { return paddleRightHigh; }

    static void setGear(int _gear) {
        gear = _gear;
        if (gear < 1)
            gear = 1;
        if (gear > totalGears)
            gear = totalGears;
    }

    static int getGear() {
        return gear;
    }

    static int getMaxGear() {
        return totalGears;
    }

private:
    int8_t throttle = 0; // -100 <-> 100
    int8_t steering = 0; // -100 <-> 100

    int8_t targetThrottle = 0;
    int8_t lerpedSpeed = 0;

    static int gear;
    static const int totalGears = 3;

    int paddleLeftLow;
    int paddleLeftHigh;
    int paddleRightLow;
    int paddleRightHigh;

    int calculateThrottle();

    int calculateSteering();

    float map(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    float lerp(float a, float b, float f) {
        return a * (1.0 - f) + (b * f);
    }

    long lastMillis = 0;
};


#endif //DRIVING_H
