/*
This is the simple text start screen. It just displays text
 */

// ------------- REPLACE function startupScreen() WITH THIS ------------/

// Run when steering wheel starts up.
void Screens::startupScreen() {
 u8g2.begin();
 u8g2.setFont(mainFont);
 u8g2.clearBuffer();

 u8g2.drawStr(10, 10, "Drive ...");
 delay(2000);
 u8g2.sendBuffer();
}