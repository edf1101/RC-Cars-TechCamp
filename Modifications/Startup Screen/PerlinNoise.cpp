/*
This is the perlin noise start screen. It does a 3d perlin noise animation then displays a name.
 */

// ------------- REPLACE function startupScreen() WITH THIS ------------/

// Run when steering wheel starts up.
void Screens::startupScreen() {
 u8g2.begin();
 u8g2.setFont(mainFont);
 randomSeed(analogRead(A0));
 int offsetX = random(1, 1000);
 int offsetY = random(1, 1000);

 float freq = 0.02f;
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