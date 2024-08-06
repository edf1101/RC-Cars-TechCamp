#include "Arduino.h"
#include "src/Screens.h"
#include "src/FPV/FpvTransmitter.h"
#include "src/Menu/Menu.h"
#include <string>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


// Run when steering wheel starts up.
void Screens::startupScreen() {
    u8g2.begin();
    u8g2.setFont(mainFont);
    u8g2.clearBuffer();

    u8g2.drawStr(10, 10, "Drive ...");
    delay(2000);
    u8g2.sendBuffer();
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
    u8g2.setFont(u8g2_font_6x10_tr  );
    u8g2.drawStr(92, 58, std::to_string(throttle).c_str());
    u8g2.setFont(mainFont);
    float angle = -PI / 2.0 - (abs(throttle) / 100.0 * PI);
    drawLineAtAngle(&u8g2, 97, 58, 20, angle, 8);

    // Draw Gear Info
    int gear = 1; // keep it at 1 for now
    if (throttle == 0) gear = 0;
    std::string gearInfo = (throttle > 0)
                               ? "F" + std::to_string(gear)
                               : ((throttle < 0) ? "R" + std::to_string(gear) : "N");

    u8g2.drawStr(17, 29, "Gear");
    u8g2.setFont(u8g2_font_6x10_tr   );
    u8g2.drawStr(27, 58, gearInfo.c_str());
    u8g2.setFont(mainFont);
    angle = -PI / 2.0 - (abs(gear) / 3.0 * PI);
    drawLineAtAngle(&u8g2, 32, 58, 20, angle, 8);
    u8g2.sendBuffer();
}
