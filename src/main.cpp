// Code for the transmitter (steering wheel) of the FPV Rally Racing car

// To compile - first setup arduino as follows:
// File | Preferences -> then add this url into additional board managers: https://dl.espressif.com/dl/package_esp32_index.json
// Tools | Board | Board Manager -> search for ESP32 and intstall
// Tools | Board | ESP32 Dev Module
// Requires external libraries:
// (from Tools/Manage Libraries): Adafruit MPU6050 (plus BUSIO), U8G2 library for graphics, MD_REncoder for encoder knob, ESP32Servo (for receiver)
// (To download manually): FastCRC

#include <Arduino.h>
#include "src/FPV/FpvTransmitter.h"

FpvTransmitter fpvTransmitter;

void setup() {
    fpvTransmitter.fpvSetup(); // Sets up the hardware and runs a test mode if one of the bottom paddles is pressed
}

void loop() {
    fpvTransmitter.fpvLoop(); // call the loop function
}
