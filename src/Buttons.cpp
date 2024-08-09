#include "Arduino.h"
#include "src/FPV/FpvTransmitter.h"
#include "src/Buttons.h"
#include "src/Driving.h"


void Buttons::handleButtons(int *throttleMax, int *steeringMax) {
    if (digitalRead(FpvTransmitter::d1) == LOW) {
        // button 1 pressed
    }
    if (digitalRead(FpvTransmitter::d2) == LOW) {
        //button 2 pressed
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
    switch1.held = (digitalRead(FpvTransmitter::d1) == LOW);
    switch2.held = (digitalRead(FpvTransmitter::d2) == LOW);
    switch3.held = (digitalRead(FpvTransmitter::d3) == LOW);
    switch4.held = (digitalRead(FpvTransmitter::d4) == LOW);
    switch5.held = (digitalRead(FpvTransmitter::d5) == LOW);
    switch6.held = (digitalRead(FpvTransmitter::d6) == LOW);
}

Buttons::Buttons(): switch1(buttonSwitch(FpvTransmitter::d1)),
                    switch2(buttonSwitch(FpvTransmitter::d2)),
                    switch3(buttonSwitch(FpvTransmitter::d3)),
                    switch4(buttonSwitch(FpvTransmitter::d4)),
                    switch5(buttonSwitch(FpvTransmitter::d5)),
                    switch6(buttonSwitch(FpvTransmitter::d6)) {
}
