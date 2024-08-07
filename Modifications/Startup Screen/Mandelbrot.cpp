/*
This is the mandelbrot start screen. It slowly creates a mandelbrot set.
 */

// ------------- REPLACE function startupScreen() WITH THIS ------------/

// Run when steering wheel starts up.
void Screens::startupScreen() {
  u8g2.begin();

  u8g2.clearBuffer();
  for (int x = 0; x < 128; x+=1) {
   for (int y = 0; y < 64; y+=1) {
    // set the initial coords of the mandel point
    // (float(x) - 0.0) * (1.0 - -1.0) / (128.0 - 0.0) + -1.0;
    float cX = (float(x) - 0.0) * (1.0 - -1.0) / (64.0 - 0.0) + -1.0;
    float cY = (float(y) - 0.0) * (1.0 - -1.0) / (32.0 - 0.0) + -1.0;

    float xPos = 0;
    float yPos = 0;

    bool valid = true;
    for (int iter = 0; iter < 20; iter++) {
     float tempX = (xPos * xPos) - (yPos * yPos) + cX;
     float tempY =( 2 * xPos * yPos) + cY;
     xPos = tempX;
     yPos = tempY;
     if((xPos*xPos) + (yPos*yPos)>4) {
      valid = false;
      break;
     }
    }
    if (valid) {
     u8g2.drawPixel(x,y);
    }

   }
  }
  delay(8000);
  u8g2.sendBuffer();
 }
