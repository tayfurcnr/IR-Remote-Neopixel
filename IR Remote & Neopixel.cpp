  // Library for Arduino
  #include <Arduino.h>

  // Library for analogWrite
  #include <analogWrite.h>

  // Library for MultiTasking
  #include <TaskScheduler.h>

  Scheduler st;

  void handlerSerial();
  void handlerIR();
  void controlButton();
  void controlLED();
  void driveMotor();

  // Functions for LED
  void colorWipe(uint32_t color, int wait);
  void theaterChase(uint32_t color, int wait);
  void rainbow(int wait);
  void theaterChaseRainbow(int wait);

  // Tasks
  Task taskHandlerSerial(300, TASK_FOREVER, &handlerSerial); // Normal 200
  Task taskHandlerIR(200, TASK_FOREVER, &handlerIR);
  Task taskControlButton(150, TASK_FOREVER, &controlButton);  // Normal 150
  Task taskControlLED(100, TASK_FOREVER, &controlLED);  // Normal 100
  Task taskDriveMotor(50, TASK_FOREVER, &driveMotor); // Normal 50

  // Library for Remote Control
  #include <IRremote.h>

  // Definitions for Remote Control
  #define RECV_PIN 14  // Pin of IR // Arduino Nano/Uno -> 5 // ESP32 -> 14

  // Object for Remote Control
  IRrecv irrecv(RECV_PIN);
  decode_results results;

  // Definitions for Buttons of Remote Control
  #define CH1 0xFFA25D
  #define CH 0xFF629D
  #define CH2 0xFFE21D
  #define PREV 0xFF22DD
  #define NEXT 0xFF02FD
  #define PLAYPAUSE 0xFFC23D
  #define VOL1 0xFFE01F
  #define VOL2 0xFFA857
  #define EQ 0xFF906F
  #define BUTON0 0xFF6897
  #define BUTON100 0xFF9867
  #define BUTON200 0xFFB04F
  #define BUTON1 0xFF30CF
  #define BUTON2 0xFF18E7
  #define BUTON3 0xFF7A85
  #define BUTON4 0xFF10EF
  #define BUTON5 0xFF38C7
  #define BUTON6 0xFF5AA5
  #define BUTON7 0xFF42BD
  #define BUTON8 0xFF4AB5
  #define BUTON9 0xFF52AD

  // Library for NeoPixel
  #include <Adafruit_NeoPixel.h>

  // Definitions for NeoPixel
  #define LED_PIN 13 // Pin of LED // Arduino Nano/Uno -> 6 // ESP32 -> 13
  #define LED_COUNT 12 // Number of LED

  // Definitions for DC Motor
  #define DC_MOTOR_PIN 3 // Pin of DC Motor // // Arduino Nano/Uno -> 3 // ESP32 
  #define DC_MOTOR_SPEED_DEFAULT 30 // Default speed of DC Motor (0-255)
  #define DC_MOTOR_SPEED_MIN 0 // 

  int LED_BRIGHTNESS_DEFAULT = 250; // Default LED Brightness
  int LED_BRIGHTNESS_MAX = 250; // LED Brightness MAX
  int LED_BRIGHTNESS_MIN = 0;  // LED Brightness MIN

  // Object for NeoPixel
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

  /*----SETTINGS----Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400)---*/
  /*    Argument 1 = Number of pixels in NeoPixel strip                                 */
  /*    Argument 2 = Arduino pin number (most are valid)                                */
  /*    Argument 3 = Pixel type flags, add together as needed:                          */
  /*       1-) NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)     */
  /*       2-) NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers) */
  /*       3-) NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)  */
  /*       4-) NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2) */
  /*       5-) NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products) */
  /*------------------------------------------------------------------------------------*/

  char incomingCommand;

  bool MOTOR_STATE = false;
  // Definition for DEBUG (True = 1 or False = 0)
  #define DEBUG 0

  void setup() {
    Serial.begin(115200); // FOR DEBUG

    // We add the tasks
    st.addTask(taskHandlerSerial);
    st.addTask(taskHandlerIR);
    st.addTask(taskControlButton);
    st.addTask(taskControlLED);
    st.addTask(taskDriveMotor);

    // We activate the tasks
    taskHandlerSerial.enable();
    taskHandlerIR.enable();
    taskControlButton.enable();
    taskControlLED.enable();
    taskDriveMotor.disable();

    // We activate the LED
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNESS_DEFAULT);

    // We activate the IR
    irrecv.enableIRIn();

    // We activate the DEBUG
    if (DEBUG)
      Serial.println("System is ready!");

    // Initial State = Rainbow Format
    incomingCommand = 'O';
  }

  void loop() {
    // It is necessary to run the runner on each loop
    st.execute();
  }

  // Function for Control 2 System
  void controlLED() {
    switch (incomingCommand) {
      
      case 'R':
        if (DEBUG)
          Serial.println("Red");
        colorWipe(strip.Color(255,  0,   0), 50); // Red
      break;
      
      case 'G':
        if (DEBUG)
          Serial.println("Green");
        colorWipe(strip.Color(  0, 255,   0), 50); // Green
      break;

      case 'B':
        if (DEBUG)
          Serial.println("Blue");
        colorWipe(strip.Color(  0,   0, 255), 50); // Blue
      break;

      case 'W':
        if (DEBUG)
          Serial.println("White & LED On");
        colorWipe(strip.Color(  127,   127, 127), 50); // White
      break;

      // System Off
      case 'O':
        if (DEBUG)
          Serial.println("LED Off");
        colorWipe(strip.Color(  0,  0,  0), 50); // Turn Off
      break;

      case '7':
        if (DEBUG)
          Serial.println("Rainbow");
        rainbow(10);             // Flowing rainbow cycle along the whole strip
      break;

      case '8':
        if (DEBUG)
          Serial.println("Rainbow-Enhanced");
        theaterChaseRainbow(100); // Rainbow-enhanced theaterChase variant
      break;

      case '4':
        if (DEBUG)
          Serial.println("Red-Enhanced");
        theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
      break;

      case '5':
        if (DEBUG)
          Serial.println("Green-Enhanced");
        theaterChase(strip.Color(  0,   127, 0), 50); // Green, half brightness
      break;

      case '6':
        if (DEBUG)
          Serial.println("Blue-Enhanced");
        theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
      break;

      case '9':
        if (DEBUG)
          Serial.println("White-Enhanced");
        theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
      break;

      case 'M':        
        if (DEBUG)
          Serial.println("Motor Off");
        taskDriveMotor.disable();
        incomingCommand = '7';
      break;

      case 'N':
        if (DEBUG)
          Serial.println("Motor On");
        taskDriveMotor.enable();
        incomingCommand = '7';
      break;

      case 'P':
        if (DEBUG)
          Serial.println("Purple");
        colorWipe(strip.Color(  255, 0,  175), 50); // Purple
      break;

      case 'V':
        if (DEBUG)
          Serial.println("Orange");
        colorWipe(strip.Color(  211, 55,  0), 50); // Orange
      break;

      }
  }

  // Function for Serial Control
  void handlerSerial() {
    if (Serial.available())
    {
      incomingCommand = Serial.read();
    }
  }

  // Function for Remote Control
  void handlerIR() {
    if (irrecv.decode(&results)) {
      irrecv.resume();
    }
  }

  // Function for Remote Control
  void controlButton() {
    if (CH1 == results.value)
      incomingCommand = 'O'; // LED Off
    else if (PREV == results.value)
      incomingCommand = 'M'; // Motor Off
    else if (PLAYPAUSE == results.value)
      incomingCommand = 'N'; // Motor On
    else if (CH2 == results.value)
      incomingCommand = 'W'; // White & LED On
    else if (BUTON1 == results.value)
      incomingCommand = 'R'; // Red
    else if (BUTON2 == results.value)
      incomingCommand = 'G'; // Green
    else if (BUTON3 == results.value)
      incomingCommand = 'B'; // Blue
    else if (BUTON4 == results.value)
      incomingCommand = '4'; // Red, half brightness
    else if (BUTON5 == results.value)
      incomingCommand = '5'; // Green, half brightness
    else if (BUTON6 == results.value)
      incomingCommand = '6'; // Blue, half brightness
    else if (BUTON7 == results.value)
      incomingCommand = '7'; // Raninbow
    else if (BUTON8 == results.value)
      incomingCommand = '8'; // Rainbow-Enhanced
    else if (BUTON9 == results.value)
      incomingCommand = '9'; // White, half brightness
    else if (EQ == results.value)
      incomingCommand = 'P'; // Purple
    else if (VOL2 == results.value)
      incomingCommand = 'V'; // Orange
    else if ( (CH == results.value) || (VOL1 == results.value) || (BUTON0 == results.value)
     || (BUTON100 == results.value) || (BUTON200 == results.value) )
      incomingCommand = 'W'; // Programmable Buttons
    else;
      // Nothing!
  }
  // Function for NeoPixel
  void colorWipe(uint32_t color, int wait) {
    strip.setBrightness(LED_BRIGHTNESS_DEFAULT);
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      delay(wait);                           //  Pause for a moment
    }
  }

  // Function for NeoPixel
  void theaterChase(uint32_t color, int wait) {
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }

  // Function for NeoPixel
  void rainbow(int wait) {
    for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }

  // Function for NeoPixel
  void theaterChaseRainbow(int wait) {
    int firstPixelHue = 0;     // First pixel starts at red (hue 0)
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)-
      for(int c=b; c<strip.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }

  // Function for DC Motor
  void driveMotor() {
    if (DEBUG)
      Serial.println("Motor Running!");
      analogWrite(DC_MOTOR_PIN, DC_MOTOR_SPEED_DEFAULT);
  }