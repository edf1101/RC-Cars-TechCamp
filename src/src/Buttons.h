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

    bool move1 = false;
    bool move2 = false;
    bool move3 = false;
    bool move4 = false;
    bool move5 = false;
    bool move6 = false;
};



#endif //BUTTONS_H
