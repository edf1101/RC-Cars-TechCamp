#ifndef BUTTONS_H
#define BUTTONS_H

class buttonSwitch {
public:
    int held;
    int ON;
    char button;

    explicit buttonSwitch(char btn) {
        button = btn;
        ON = false;
        held = false;
    }

    void flip() {
        if (ON) {
            ON = false;
        } else {
            ON = true;
        }
    }
    void toggle() {
        if (!held) {
            flip();
            held = true;
        }
    }
};

class Buttons {



public:
    Buttons();
    void handleButtons(int *throttleMax, int *steeringMax);
    void checkButtonHeld();

    // Initalise buttons
    buttonSwitch switch1;
    buttonSwitch switch2;
    buttonSwitch switch3;
    buttonSwitch switch4;
    buttonSwitch switch5;
    buttonSwitch switch6;

};



#endif //BUTTONS_H
