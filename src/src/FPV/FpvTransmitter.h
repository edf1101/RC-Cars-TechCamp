#ifndef FPVTRANSMITTER_H
#define FPVTRANSMITTER_H

#include "../Menu/Menu.h"  // For the menu system
#include "../Radio/Radio.h" // For the radio
#include <Wire.h>  // This and the next two are for the motion sensor
#include <Adafruit_MPU6050.h>
#include <Preferences.h> // This one for storing configuration to non-volatile memory (not lost when you power off)
#include <functional>
#include "../Screens.h"
#include "../Buttons.h"
#include "../Driving.h"

#define mainFont u8g2_font_7x13B_mr

class FpvTransmitter {
public:
    // Pin definitions for switches and magnetic sensors
    static const char swA=16;
    static const char swB=13;
    static const char mag1=32;
    static const char mag2=35;
    static const char d1=5;
    static const char d2=14;
    static const char d3=23;
    static const char d4=19;
    static const char d5=18;
    static const char d6=26;
    static const char d7=15;
    static const char d8=34;
    static const char d9=33;



    MenuItem *mainMenu;
    Buttons buttonsSystem;
    Driving drivingControls;

    void fpvSetup();
    void fpvLoop();
    void calibrateThrottle();
    int isCalibrated();
    int getSteering(int steeringMax);
    void doTest();
    void bindIfNeeded();

    Adafruit_MPU6050 mpu;

    Preferences preferences;
    static int throttleMax;
    static int rawTilt; // The raw ish -90 <-> 90 steering value
    int throttleLow; // The magnetic sensor value when throttle not pressed in
    int throttleHigh; // The magnetic sensor value when throttle pressed in
    uint16_t unitNum = 0;
    // The number of my car.  It is a 15 bit number that is randomly generated by the transmitter the first time you go into race mode after powering on


    void packetRx();
    void menuChange(const char *itemName, int value);
    Screens screens;
};



#endif //FPVTRANSMITTER_H
