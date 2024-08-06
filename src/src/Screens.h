#ifndef SCREENS_H
#define SCREENS_H

#include "../Images.h"
#include <U8g2lib.h>

#define mainFont u8g2_font_7x13B_mr

class Screens {
public:
    // Setup constants
    const int DISPLAY_HEIGHT = 64;
    const int DISPLAY_WIDTH = 128;
    const int CENTER_X = DISPLAY_WIDTH / 2;
    const int CENTER_Y = DISPLAY_HEIGHT / 2;

    // Functions to help center screen bits.
    int center_w(int width) const {
        return CENTER_X - width / 2;
    }

    int center_h(int height) const {
        return CENTER_Y - height / 2;
    }

    void startupScreen();

    void raceScreen();

    void drawLineAtAngle(U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2, int centerX, int centerY, int radius, float angle,
                         int lineOffset = 0) {
        int endX = centerX + radius * sin(angle);
        int endY = centerY + radius * cos(angle);

        if (lineOffset != 0) {
            centerX += lineOffset * sin(angle);
            centerY += lineOffset * cos(angle);
        }
        u8g2->drawLine(centerX, centerY, endX, endY);
    }
};


#endif //SCREENS_H
