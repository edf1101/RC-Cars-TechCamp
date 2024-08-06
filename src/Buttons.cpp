#include "Arduino.h"
#include "src/FPV/FpvTransmitter.h"
#include "src/Buttons.h"


void Buttons::handleButtons(int *throttleMax, int *steeringMax) {
    if (digitalRead(FpvTransmitter::d1) == LOW) {
        // Button 1 pressed.

        // Example code:
        // *throttleMax = 100;
        // *steeringMax = 90;
    }
    if (digitalRead(FpvTransmitter::d2) == LOW) {
        // Button 2 pressed.
    }
    if (digitalRead(FpvTransmitter::d3) == LOW) {
        // Button 3 pressed.
    }
    if (digitalRead(FpvTransmitter::d4) == LOW) {
        // Button 4 pressed.
    }
    if (digitalRead(FpvTransmitter::d5) == LOW) {
        // Button 5 pressed.
    }
    if (digitalRead(FpvTransmitter::d6) == LOW) {
        // Button 6 pressed.
    }
}




void Buttons::checkButtonHeld() {
    if (digitalRead(FpvTransmitter::d1) == HIGH)
        switch1.held = 0;
    if (digitalRead(FpvTransmitter::d2) == HIGH)
        switch2.held = 0;
    if (digitalRead(FpvTransmitter::d3) == HIGH)
        switch3.held = 0;
    if (digitalRead(FpvTransmitter::d4) == HIGH)
        switch4.held = 0;
    if (digitalRead(FpvTransmitter::d5) == HIGH)
        switch5.held = 0;
    if (digitalRead(FpvTransmitter::d6) == HIGH)
        switch6.held = 0;
}

Buttons::Buttons(): switch1(buttonSwitch(FpvTransmitter::d1)),
                    switch2(buttonSwitch(FpvTransmitter::d2)),
                    switch3(buttonSwitch(FpvTransmitter::d3)),
                    switch4(buttonSwitch(FpvTransmitter::d4)),
                    switch5(buttonSwitch(FpvTransmitter::d5)),
                    switch6(buttonSwitch(FpvTransmitter::d6)) {}
