#include "FpvTransmitter.h"

// Contains some common code for the FPV Rally Racing car  - primarily to setup hardware, transmit binding info, handle the throttle and steering and run a test
int FpvTransmitter::throttleMax = 0;
int FpvTransmitter::rawTilt = 0;

void FpvTransmitter::fpvSetup() {
    // Configure the switches - A and B are the bottom paddles, D1-6 are the top red buttons, and D7-9 are spares on the PCB for future use
    pinMode(swA, INPUT_PULLUP);
    pinMode(swB, INPUT_PULLUP);
    pinMode(d1, INPUT_PULLUP);
    pinMode(d2, INPUT_PULLUP);
    pinMode(d3, INPUT_PULLUP);
    pinMode(d4, INPUT_PULLUP);
    pinMode(d5, INPUT_PULLUP);
    pinMode(d6, INPUT_PULLUP);
    pinMode(d7, INPUT_PULLUP);
    pinMode(d8, INPUT_PULLUP);
    pinMode(d9, INPUT_PULLUP);

    screens.startupScreen();

    // Setup the accelerometer to detect steering wheel tilt ...
    Wire.begin(27, 22); // It's an I2C device so needs this
    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // valid options 2,4,8,16
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    // valid options 250,500,1000,2000 degrees/sec - We're not currently using the gyro - just the accelerometer
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    // valid options 5,10,21,44,94,184,260 hz - think this is for the gyro again

    // Load T0 and T1 from non-volatile memory (these are the calibration constants for the throttle)
    preferences.begin("FPV", false);

    // set up the range of the paddles
    drivingControls.setTriggerLeftRange(preferences.getInt("PadLefLo", 0), preferences.getInt("PadLefHi", 0));
    drivingControls.setTriggerRightRange(preferences.getInt("PadRightLo", 0), preferences.getInt("PadRightHi", 0));

    preferences.end();

    // If either bottom paddle pressed, go into test mode
    if ((digitalRead(swA) == LOW) || (digitalRead(swB) == LOW))
        doTest();

    // This sets up the menu system.  Items on the main menu either just execute code or allow parameters to be set
    // The ones below with numbers are parameters
    // For example, the line:  mainMenu->addItem("Config","Throttle",5,100,30,5); ...
    // .. sets up a parameter called 'Throttle' that can go between 5 and 100, with a default value of 30, and an increment of 5
    std::function<void(const char *, int)> func = std::bind(&FpvTransmitter::menuChange, this, std::placeholders::_1,
                                                            std::placeholders::_2);
    mainMenu = new MenuItem(func, "Main Menu");
    mainMenu->addItem("Main Menu", "Race!");
    mainMenu->addItem("Main Menu", "Config");
    mainMenu->addItem("Config", "Throttle", 5, 100, 30, 5);
    mainMenu->addItem("Config", "Steering", 5, 90, 60, 5);
    mainMenu->addItem("Config", "Calibrate");
    mainMenu->addItem("Config", "Back");
    mainMenu->addItem("Main Menu", "Test");
    mainMenu->addItem("Main Menu", "Back");

    mainMenu->activate();

    setupRadio(1, std::bind(&FpvTransmitter::packetRx, this));
    // Sets up the radio, and calls packetRx() when it receives a command
}

void FpvTransmitter::fpvLoop() {
    menuService();
}

void FpvTransmitter::packetRx()
// This gets called when a packet is received from the radio - it's blank because the transmitter is a transmitter, not a receiver!
{
}

void FpvTransmitter::menuChange(const char *itemName, int value)
// This gets called when a menu item is selected or a parameter is changed
{
    delay(300); // have just hit select, so give time to release ...
    if ((strcmp("Race!", itemName) == 0) && isCalibrated()) {
        throttleMax = mainMenu->getItem("Throttle")->defaultVar;
        int steeringMax = mainMenu->getItem("Steering")->defaultVar;
        bindIfNeeded();
        while (digitalRead(ENC_SW) == HIGH) // While encoder knob not pressed ...
        {
            u8g2.clearBuffer(); // Refresh screen.

            // Check if any red buttons pressed, and if so put the button number in the 'command' parameter to send to the receiver
            if (digitalRead(d1) == LOW && buttonsSystem.move1 == true)
                outgoingRadioPacket.command = 1;
            else if (digitalRead(d2) == LOW && buttonsSystem.move2 == true)
                outgoingRadioPacket.command = 2;
            else if (digitalRead(d3) == LOW && buttonsSystem.move3 == true)
                outgoingRadioPacket.command = 3;
            else if (digitalRead(d4) == LOW && buttonsSystem.move4 == true)
                outgoingRadioPacket.command = 4;
            else if (digitalRead(d5) == LOW && buttonsSystem.move5 == true)
                outgoingRadioPacket.command = 5;
            else if (digitalRead(d6) == LOW && buttonsSystem.move6 == true)
                outgoingRadioPacket.command = 6;
            else
                outgoingRadioPacket.command = 0;

            buttonsSystem.handleButtons(&throttleMax, &steeringMax);

            rawTilt = getSteering(steeringMax);
            drivingControls.calculateDriving();
            outgoingRadioPacket.throttle = drivingControls.getThrottle();
            outgoingRadioPacket.angle = drivingControls.getSteering();

            screens.raceScreen();

            buttonsSystem.checkButtonHeld();
            sendPacket();
            delay(5 + random(0, 11));
            // delay for an average of 10ms +/- 5ms - this variation prevents regular collisions
        }
    }

    if (strcmp("Test", itemName) == 0)
        doTest();
    if (strcmp("Calibrate", itemName) == 0)
        calibrateThrottle();
    delay(300); // if they've hit select, give time to release before continuing with menu system
}

void FpvTransmitter::bindIfNeeded()
// If we've not come up with a random unit number for this transmitter, do so, and transmit it to a listening receiver
{
    int i;

    if (unitNum == 0) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_7x13B_mr);
        u8g2.drawStr(0, 10, "Binding ...");
        u8g2.sendBuffer();
        randomSeed(millis());
        unitNum = 0;
        while (unitNum == 0)
            // unit must not be zero as that indicates it is not bound yet, so just keep generating random numbers until it isn't zero! ...
            unitNum = random(0, 65535) & 0x7fff; // get rid of top bit as that indicates binding mode

        outgoingRadioPacket.unitNum = unitNum | 0x8000; // Now set top bit and broadcast for 1 second to bind
        for (i = 1; i <= 100; i++) {
            sendPacket();
            delay(10);
        }
        outgoingRadioPacket.unitNum = unitNum;
        // Get rid of top bit again for normal driving as we should now have bound
    }
}

int FpvTransmitter::isCalibrated() // Returns 0 and displays message if throttle not calibrated yet
{
    if ((drivingControls.getPaddleRightLow() == 0) || (drivingControls.getPaddleLeftLow() == 0) || (
            drivingControls.getPaddleLeftHigh() == 0) || (drivingControls.getPaddleRightHigh() == 0)) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_7x13B_mr);
        u8g2.drawStr(0, 10, "Please Calibrate");
        u8g2.drawStr(0, 24, "from Config Menu");
        u8g2.sendBuffer();
        delay(2000); // Delay long enough to read message
        return 0;
    } else
        return 1;
}

void FpvTransmitter::calibrateThrottle() // Calibrates the throttle
{
    int tempLeftTrigLow = analogRead(mag2); // this is the magnetic sensor reading with no throttle
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_mr);
    u8g2.drawStr(0, 10, "Floor Left Trigger");
    u8g2.drawStr(0, 24, "and press bottom");
    u8g2.drawStr(0, 38, "left paddle ...");
    u8g2.sendBuffer();
    while (digitalRead(swA) == HIGH); // Wait until bottom left paddle is hit
    int tempLeftTrigHigh = analogRead(mag1); // this is the sensor readng with full throttle
    u8g2.clearBuffer();
    int tempRightTrigLow = analogRead(mag2); // this is the magnetic sensor reading with no throttle
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_mr);
    u8g2.drawStr(0, 10, "Floor Right Trigger");
    u8g2.drawStr(0, 24, "and press bottom");
    u8g2.drawStr(0, 38, "right paddle ...");
    u8g2.sendBuffer();
    while (digitalRead(swB) == HIGH); // Wait until bottom left paddle is hit
    int tempRightTrigHigh = analogRead(mag2); // this is the sensor readng with full throttle
    // Now store these calibration values in non-volatile memory
    preferences.begin("FPV", false);
    preferences.putInt("PadLefLo", tempLeftTrigLow);
    preferences.putInt("PadLefHi", tempLeftTrigHigh);
    preferences.putInt("PadRightLo", tempRightTrigLow);
    preferences.putInt("PadRightHi", tempRightTrigHigh);
    preferences.end();
    drivingControls.setTriggerLeftRange(tempLeftTrigLow,tempLeftTrigHigh);
    drivingControls.setTriggerRightRange(tempRightTrigLow,tempRightTrigHigh);

}

int FpvTransmitter::getSteering(int steeringMax) // returns value for steering position
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    int angle = a.acceleration.y * 10;
    return map(angle, 100, -100, 90 - steeringMax, 90 + steeringMax);
}

void FpvTransmitter::doTest() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13B_mr);
    while (digitalRead(ENC_SW) == HIGH) {
        u8g2.clearBuffer(); // clear the internal memory
        // Show if any of the bottons pressed ...
        if ((digitalRead(swA) == LOW)) { u8g2.drawStr(8 * 0, 10, "A"); }
        if ((digitalRead(swB) == LOW)) { u8g2.drawStr(8 * 1, 10, "B"); }
        if ((digitalRead(d1) == LOW)) { u8g2.drawStr(8 * 2, 10, "1"); }
        if ((digitalRead(d2) == LOW)) { u8g2.drawStr(8 * 3, 10, "2"); }
        if ((digitalRead(d3) == LOW)) { u8g2.drawStr(8 * 4, 10, "3"); }
        if ((digitalRead(d4) == LOW)) { u8g2.drawStr(8 * 5, 10, "4"); }
        if ((digitalRead(d5) == LOW)) { u8g2.drawStr(8 * 6, 10, "5"); }
        if ((digitalRead(d6) == LOW)) { u8g2.drawStr(8 * 7, 10, "6"); }
        int reading;
        reading = analogRead(39); // IO 39 is ocnnected to voltage divider from battery so we can measure it's voltage
        float voltage;
        // The following code makes the voltage reading more linear as the ADC in the ESP32 is not great!
        voltage = -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 *
                  pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
        reading = voltage * 20;
        // Now display the battery voltage ...
        static char buf[10];
        buf[0] = (reading / 10) + '0';
        buf[1] = '.';
        buf[2] = (reading % 10) + '0';
        buf[3] = 'V';
        buf[4] = 0;
        u8g2.drawStr(0, 23, buf);

        // Now get the motion sensor data ...
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        // ... and print angle of steering wheel ...
        u8g2.drawStr(0, 38, "Tilt:");
        u8g2.setCursor(70, 36);
        u8g2.print(a.acceleration.y);

        // ... and display raw values of top paddles
        reading = analogRead(mag1);
        u8g2.drawStr(0, 49, "Mag1:");
        u8g2.setCursor(70, 49);
        u8g2.print(reading);

        reading = analogRead(mag2);
        u8g2.drawStr(0, 62, "Mag2:");
        u8g2.setCursor(70, 62);
        u8g2.print(reading);

        u8g2.sendBuffer(); // Finally display what we have drawn on the OLED screen
        delay(50);
    }
}
