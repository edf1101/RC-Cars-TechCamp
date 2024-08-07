/*
This is the default code for the Race screen. Just containing two numbers for steering and throttle.
 */



// ------------------------ REPLACE THE raceScreen() FUNCTION WITH THIS --------------------------------//


// Edit what is shown on the race screen here.
void Screens::raceScreen() {

    u8g2.clearBuffer();
    u8g2.setCursor(0,10);
    u8g2.setFont(mainFont);
    u8g2.print(outgoingRadioPacket.throttle);
    u8g2.setCursor(0,24);
    u8g2.print(outgoingRadioPacket.angle);

    u8g2.sendBuffer();
}