
#ifndef DRIVING_H
#define DRIVING_H

#include "Arduino.h"

class Driving {
    public:
    void calculateDriving(){

        throttle = calculateThrottle();
        steering = calculateSteering();

    }
        int8_t getThrottle() const{return throttle;}
        int8_t getSteering() const{return steering;}

        void setTriggerLeftRange(int low, int high){
            paddleLeftHigh= high;
            paddleLeftLow = low;
        }
        void setTriggerRightRange(int low, int high){
            paddleRightHigh= high;
            paddleRightLow = low;
        }

        int getPaddleLeftLow() const{return paddleLeftLow;}
        int getPaddleLeftHigh() const{return paddleLeftHigh;}
        int getPaddleRightLow() const{return paddleRightLow;}
        int getPaddleRightHigh() const{return paddleRightHigh;}

    private:
        int8_t throttle =0; // -100 <-> 100
        int8_t steering = 0; // -100 <-> 100

        int paddleLeftLow;
        int paddleLeftHigh;
        int paddleRightLow;
        int paddleRightHigh;

        int calculateThrottle();
        int calculateSteering();
};



#endif //DRIVING_H
