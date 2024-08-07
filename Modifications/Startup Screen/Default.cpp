/*
This is the default start screen. It does nothing.
 */

// ------------- REPLACE function startupScreen() WITH THIS ------------/

// Run when steering wheel starts up.
void Screens::startupScreen() {
 u8g2.begin();

 u8g2.sendBuffer();
 delay(2000);
}