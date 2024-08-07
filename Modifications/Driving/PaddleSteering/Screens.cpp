#include "Arduino.h"
#include "src/Screens.h"
#include "src/FPV/FpvTransmitter.h"
#include "src/Menu/Menu.h"
#include <string>
#include "src/Perlin.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


// Run when steering wheel starts up.
void Screens::startupScreen() {
    u8g2.begin();
    u8g2.setFont(mainFont);
    randomSeed(analogRead(A0));
    int offsetX = random(1, 1000);
    int offsetY = random(1, 1000);

    float freq = 0.05f;
    for (int z = 0; z < 110; z++) {
        u8g2.clearBuffer();
        for (int x = 0; x < 128; x++) {
            for (int y = 0; y < 64; y++) {
                float pValue = pnoise(float(offsetX + x) * freq, float(offsetY + y) * freq, float(z) * freq);
                if ((pValue > 0 && z < 90) || (z >= 90 && pValue > -float(z - 90) / 20.0)) {
                    u8g2.drawPixel(x, y);
                }
            }
        }
        u8g2.sendBuffer();
        delay(20);
    }

    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.drawStr(30, 30, "My Car");
    u8g2.setFont(mainFont);

    u8g2.sendBuffer();
    u8g2.setDrawColor(1);
    delay(2000);
}

// Edit what is shown on the race screen here.
void Screens::raceScreen() {
    int throttle = outgoingRadioPacket.throttle;
    int steering = outgoingRadioPacket.angle;

    u8g2.clearBuffer();
    u8g2.setCursor(0, 0);
    u8g2.setFont(mainFont);
    u8g2.drawXBM(0, 0, 128, 64, dashboard);

    // draw throttle info
    u8g2.drawStr(79, 29, "Speed");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(92, 58, std::to_string(throttle).c_str());
    u8g2.setFont(mainFont);
    float angle = -PI / 2.0 - (abs(throttle) / 100.0 * PI);
    drawLineAtAngle(&u8g2, 97, 58, 20, angle, 8);

    // Draw Gear Info
    int gear =Driving::getGear(); // keep it at 1 for now
    if (throttle == 0) gear = 0;
    std::string gearInfo = (throttle > 0)
                               ? "F" + std::to_string(gear)
                               : ((throttle < 0) ? "R" + std::to_string(gear) : "N");

    u8g2.drawStr(17, 29, "Gear");
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(27, 58, gearInfo.c_str());
    u8g2.setFont(mainFont);
    angle = -PI / 2.0 - (abs(gear) / 3.0 * PI);
    drawLineAtAngle(&u8g2, 32, 58, 20, angle, 8);

    // draw steering line
    int maxLineLength = 40;
    int mappedSteering = map(steering, 0, 180, -maxLineLength, maxLineLength);
    //draw a box around the steering bit
    u8g2.drawFrame(64 - maxLineLength - 3, 5, (2 * maxLineLength) + 6, 10);
    // draw a middle line to represent where 0 steering is
    u8g2.drawVLine(64, 5, 10);
    // draw the line representing steering magnitude
    u8g2.drawLine(64, 9, 64 + mappedSteering, 9);
    if (mappedSteering > 0) {
        u8g2.drawTriangle(64 + mappedSteering, 13, 64 + mappedSteering, 5, 64 + mappedSteering + 3, 9);
    } else if (mappedSteering < 0) {
        u8g2.drawTriangle(64 + mappedSteering, 13, 64 + mappedSteering, 5, 64 + mappedSteering - 3, 9);
    }

    u8g2.sendBuffer();
}
