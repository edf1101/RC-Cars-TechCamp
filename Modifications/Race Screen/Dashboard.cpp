/*
This is the code for the Race screen to make it look like a dashboard.
 */



// ------------------------ REPLACE THE raceScreen() FUNCTION WITH THIS --------------------------------//


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

    u8g2.drawStr(92, 58, String(throttle).c_str());
    u8g2.setFont(mainFont);

    float angle = -PI / 2.0 - (abs(throttle) / 100.0 * PI);
    drawLineAtAngle(&u8g2, 97, 58, 20, angle, 8);

    // Draw Gear Info
    int gear =Driving::getGear(); // keep it at 1 for now
    if (throttle == 0) gear = 0;
    String gearInfo = (throttle > 0)
                               ? "F" + String(gear)
                               : ((throttle < 0) ? "R" + String(gear) : "N");

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