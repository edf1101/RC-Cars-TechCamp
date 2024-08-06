#include "src/Driving.h"

#include "src/Radio/Radio.h"
#include "src/FPV/FpvTransmitter.h"



int Driving::calculateThrottle() {
// This is the default code for working out throttle with the
// paddle switches for FWD / REV, and paddle for intensity

    int throttleMax = FpvTransmitter::throttleMax;
    int throttleRange = getPaddleRightLow() - getPaddleRightHigh();

    long currentIntensity = analogRead(FpvTransmitter::mag2);

    currentIntensity = currentIntensity - getPaddleRightHigh();
    currentIntensity = throttleMax - (throttleMax * currentIntensity / throttleRange);
    currentIntensity = abs(currentIntensity);
    int currentThrottle = currentIntensity;
    if (currentIntensity < 0) currentThrottle = 0;
    if (currentIntensity > throttleMax) currentThrottle = throttleMax;

    if (digitalRead(FpvTransmitter::swA) == LOW) // Reverse if switch A held down
        currentThrottle = -currentIntensity;
    else if (digitalRead(FpvTransmitter::swB) == HIGH)
        currentThrottle = 0; // Cut motors if nether switch held down
    return currentThrottle;

}

int Driving::calculateSteering() {
    // This is the default code for working out the steering. Just based on the tilt values

    return FpvTransmitter::rawTilt;
}

