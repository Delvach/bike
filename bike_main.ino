#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <gamma.h>
#include <Adafruit_NeoMatrix.h>
#include <Animation.h>

// Neopixels - general
#define WRGB_NEO_PTYPE  NEO_GRBW
#define RGB_NEO_PTYPE  NEO_GRB

// Neopixels - bottom front fork (rgbw strip)
#define FORK_STRIP_PIN 8
#define FRONT_STRIP_NUM 98 

// Neopixels - rear (rgb 8x8 matrix)
#define BACK_LIGHT_PIN 7

// Neopixels - handlebar circle (rgb 16-pixel circle)
#define HANDLEBAR_CIRCLE_PIN 9
#define HANDLEBAR_CIRCLE_NUM 16

// Neopixels - handlebar upper (rgbw strip)
#define HANDLEBAR_STRIP_PIN 10
#define HANDLEBAR_STRIP_NUM 36


#define LWAIT    50

#define MAX_BRIGHTNESS  64 // set max brightness


// prototype light mapping
#define HANDLEBAR_LENGTH HANDLEBAR_STRIP_NUM/2
#define HANDLBAR_CIRCLE_LENGTH HANDLEBAR_CIRCLE_NUM/2

#define FORK_TOP_LENGTH 9
#define FORK_SIDE_LENGTH 18
#define FORK_BOTTOM_LENGTH 4
#define FORK_FRONT_LENGTH 18

#define FORK_RIGHT_TOP_INDEX 0
#define FORK_RIGHT_SIDE_INDEX 9
#define FORK_RIGHT_BOTTOM_INDEX 27
#define FORK_RIGHT_FRONT_INDEX 31

#define FORK_LEFT_TOP_INDEX 49
#define FORK_LEFT_SIDE_INDEX 58
#define FORK_LEFT_BOTTOM_INDEX 76
#define FORK_LEFT_FRONT_INDEX 80





// SPECTRUM ANALYZER
#define SPECTRUMSHIELD_PIN_STROBE 4
#define SPECTRUMSHIELD_PIN_RESET 5
#define SPECTRUMSHIELD_PIN_LEFT 0 //analog
#define SPECTRUMSHIELD_PIN_RIGHT 1 //analog

int left[7]; 
int right[7];




Adafruit_NeoPixel frontforkstrip = Adafruit_NeoPixel(FRONT_STRIP_NUM, FORK_STRIP_PIN, WRGB_NEO_PTYPE + NEO_KHZ800);
Adafruit_NeoPixel handlebarstrip = Adafruit_NeoPixel(HANDLEBAR_STRIP_NUM, HANDLEBAR_STRIP_PIN, WRGB_NEO_PTYPE + NEO_KHZ800);
//Adafruit_NeoPixel handlebarcircle = Adafruit_NeoPixel(HANDLEBAR_CIRCLE_NUM, HANDLEBAR_CIRCLE_PIN, RGB_NEO_PTYPE + NEO_KHZ800);

//Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
//  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
//  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
//  NEO_GRB            + NEO_KHZ800);


/*
 * 0 - default
 * 
 */
byte mode = 0;

uint32_t _RED = frontforkstrip.Color(64, 0, 0, 0);
uint32_t _GREEN = frontforkstrip.Color(0, 127, 0, 0);
uint32_t _BLACK = frontforkstrip.Color(0, 0, 0, 0);
uint32_t _PURPLE = frontforkstrip.Color(127, 0, 127, 0);
uint32_t _WHITE = frontforkstrip.Color(0, 0, 0, 127);

#define MINPOWER 128
#define MAXPOWER 1023


#define WAITTIME 350

unsigned long timeCurrent;

Animation FrontFork_Animation = Animation(18, 20);


void setup() {
  frontforkstrip.begin();
  frontforkstrip.setBrightness(MAX_BRIGHTNESS);
  frontforkstrip.show();
  
//  backlightmatrix.begin();
//  backlightmatrix.setBrightness(MAX_BRIGHTNESS);
//  backlightmatrix.show();

//  handlebarcircle.begin();
//  handlebarcircle.setBrightness(MAX_BRIGHTNESS);
//  handlebarcircle.show();

  handlebarstrip.begin();
  handlebarstrip.setBrightness(MAX_BRIGHTNESS);
  handlebarstrip.show();

  //initialize eq
  pinMode(SPECTRUMSHIELD_PIN_RESET, OUTPUT); // reset
  pinMode(SPECTRUMSHIELD_PIN_STROBE, OUTPUT); // strobe
  digitalWrite(SPECTRUMSHIELD_PIN_RESET,LOW); // reset low
  digitalWrite(SPECTRUMSHIELD_PIN_STROBE,HIGH); //pin 5 is RESET on the shield

  setForkRightBottom(_PURPLE);
  setForkLeftBottom(_PURPLE);

//  setHandlebarStrip(_PURPLE);

//  setHandlebarCircle(handlebarcircle.Color(127, 0, 127));

  timeCurrent = millis();


  

  FrontFork_Animation.start(1);
}

void loop() {
  readSpectrum();

  setSpectrum_handlebarLeft(left[1], left[1]);
  setSpectrum_forkFrontLeft(left[2], left[3]);
  setSpectrum_forkSideLeft(left[4], left[5]);
  setSpectrum_forkTopLeft(left[5]);

  setSpectrum_handlebarRight(right[1], right[1]);
  setSpectrum_forkFrontRight(right[2], right[3]);
  setSpectrum_forkSideRight(right[4], right[5]);
  setSpectrum_forkTopRight(right[5]);

  
//  setForkLeftTopPixel

//  setSpectrum_circleLeft(left[6]);

  unsigned long timeDiff = millis() - timeCurrent;
  timeCurrent = millis();
  
//  testForkAreas();
//  testForkScroll();
//testFork();
//   if(FrontFork_Animation.isActive() && FrontFork_Animation.execute()) {
//    
//   }

  

  
}

void setSpectrum_forkTopLeft(int value) {
  int index = getNumLEDsFromValue(value, FORK_TOP_LENGTH);

  byte i;
  for(i = 0; i < FORK_TOP_LENGTH; i++) {
    uint32_t color = i < index ? _RED : _BLACK;
    setForkLeftTopPixel(i, color);
  }
  
  
}

void setSpectrum_forkTopRight(int value) {
  int index = getNumLEDsFromValue(value, FORK_TOP_LENGTH);

  byte i;
  for(i = 0; i < FORK_TOP_LENGTH; i++) {
    uint32_t color = i < index ? _RED : _BLACK;
    setForkRightTopPixel(i, color);
  }
}

  
  


void setSpectrum_handlebarLeft(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, HANDLEBAR_LENGTH);
  
  byte i;
  for(i = 0; i < HANDLEBAR_LENGTH; i++) {
    uint32_t color = i <= index ? _RED : _BLACK;
    setHandlebarStripLeftPixel(i, color);
  }
}

void setSpectrum_handlebarRight(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, HANDLEBAR_LENGTH);
  
  byte i;
  for(i = 0; i < HANDLEBAR_LENGTH; i++) {
    uint32_t color = i <= index ? _RED : _BLACK;
    setHandlebarStripRightPixel(i, color);
  }
}

void setSpectrum_forkFrontLeft(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, FORK_FRONT_LENGTH);
  
  byte i;
  for(i = 0; i < FORK_FRONT_LENGTH; i++) {
    uint32_t color = i < index ? _PURPLE : _BLACK;
    setForkLeftFrontPixel(i, color);
  }
}

void setSpectrum_forkFrontRight(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, FORK_FRONT_LENGTH);
  
  byte i;
  for(i = 0; i < FORK_FRONT_LENGTH; i++) {
    uint32_t color = i < index ? _PURPLE : _BLACK;
    setForkRightFrontPixel(i, color);
  }
}

void setSpectrum_forkSideLeft(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, FORK_SIDE_LENGTH);
  
  byte i;
  for(i = 0; i < FORK_SIDE_LENGTH; i++) {
    uint32_t color = i < index ? _GREEN : _BLACK;
    setForkLeftSidePixel(i, color);
  }
}

void setSpectrum_forkSideRight(int value1, int value2) {
  int index = getNumLEDsFromValue(value1, FORK_SIDE_LENGTH);
  
  byte i;
  for(i = 0; i < FORK_SIDE_LENGTH; i++) {
    uint32_t color = i < index ? _GREEN : _BLACK;
    setForkRightSidePixel(i, color);
  }
}



void setSpectrum_circleLeft(int value) {
  int index = getNumLEDsFromValue(value, HANDLBAR_CIRCLE_LENGTH);

  byte i;
  for(i = 0; i < HANDLBAR_CIRCLE_LENGTH; i++) {
    uint32_t color = i < index ? _GREEN : _BLACK;
    setHandlebarCircleLeftPixel(i, color);
  }
}











// Index is 0-17 (18 total) from top to bottom
void setForkFrontPixel(uint16_t index, uint32_t color) {
  setForkLeftSidePixel(index, color);
  setForkLeftFrontPixel(index, color);
  setForkRightSidePixel(index, color);
  setForkRightFrontPixel(index, color);
}

void setForkLeftSidePixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = index + FORK_RIGHT_SIDE_INDEX;
  frontforkstrip.setPixelColor(trueIndex, color);
  frontforkstrip.show();
}

void setForkLeftFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_RIGHT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontforkstrip.setPixelColor(trueIndex, color);
  frontforkstrip.show();
}

void setForkRightSidePixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = index + FORK_LEFT_SIDE_INDEX;
  frontforkstrip.setPixelColor(trueIndex, color);
  frontforkstrip.show();
  
}

void setForkRightFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_LEFT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontforkstrip.setPixelColor(trueIndex, color);
  frontforkstrip.show();
  
}

#define TESTWAIT 5

void testFork() {
  for(uint16_t i = 0; i < 18; i++) {
    setForkFrontPixel(i, _GREEN);
//    setHandlebarStripPixel(map(i, 0, 17, 0, 15), _GREEN);
    if(i > 0) setForkFrontPixel(i-1, _BLACK);
    delay(TESTWAIT);
  }


  
  uint16_t idx = 0;
  for(uint16_t y = 0; y < 18; y++) {
    idx = 17-y;
    setForkFrontPixel(idx, _GREEN);
//    setHandlebarStripPixel(map(y, 0, 17, 0, 15), _RED);
    setForkFrontPixel(idx+1, _BLACK);
    delay(TESTWAIT);
  }

  
}

void testForkScroll() {

  setForkRightTop(_RED);
  setForkLeftTop(_RED);
  delay(WAITTIME);
  setForkRightTop(_BLACK);
  setForkLeftTop(_BLACK);

  setForkRightSide(_RED);
  setForkLeftSide(_RED);
  delay(WAITTIME);
  setForkRightSide(_BLACK);
  setForkLeftSide(_BLACK);

  setForkRightBottom(_RED);
  setForkLeftBottom(_RED);
  delay(WAITTIME);
  setForkRightBottom(_BLACK);
  setForkLeftBottom(_BLACK);

  setForkRightFront(_RED);
  setForkLeftFront(_RED);
  delay(WAITTIME);
  setForkRightFront(_BLACK);
  setForkLeftFront(_BLACK);
  
}

void testForkAreas() {
  setForkRightTop(_RED);
  delay(WAITTIME);
  setForkRightTop(_BLACK);

  setForkRightSide(_RED);
  delay(WAITTIME);
  setForkRightSide(_BLACK);

  setForkRightBottom(_RED);
  delay(WAITTIME);
  setForkRightBottom(_BLACK);

  setForkRightFront(_RED);
  delay(WAITTIME);
  setForkRightFront(_BLACK);


  setForkLeftTop(_RED);
  delay(WAITTIME);
  setForkLeftTop(_BLACK);

  setForkLeftSide(_RED);
  delay(WAITTIME);
  setForkLeftSide(_BLACK);

  setForkLeftBottom(_RED);
  delay(WAITTIME);
  setForkLeftBottom(_BLACK);

  setForkLeftFront(_RED);
  delay(WAITTIME);
  setForkLeftFront(_BLACK);

  

  
}



void setForkLeftTopPixel(uint16_t displayIndex, uint32_t color) {
  uint16_t index_start = FORK_LEFT_TOP_INDEX;
  uint16_t index_end = FORK_LEFT_TOP_INDEX + FORK_TOP_LENGTH;

  uint16_t dIndex = 0;
  for(uint16_t i = index_start; i < index_end; i++) {
    uint32_t c = (displayIndex < dIndex++) ? color : _BLACK;
    frontforkstrip.setPixelColor(i, c);
  }
  frontforkstrip.show();
}

void setForkRightTopPixel(uint16_t displayIndex, uint32_t color) {
  uint16_t index_start = FORK_RIGHT_TOP_INDEX;
  uint16_t index_end = FORK_RIGHT_TOP_INDEX + FORK_TOP_LENGTH;

  uint16_t dIndex = 0;
  for(uint16_t i = index_start; i < index_end; i++) {
    uint32_t c = (displayIndex < dIndex++) ? color : _BLACK;
    frontforkstrip.setPixelColor(i, c);
  }
  frontforkstrip.show();
}



void setForkRightTop(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_TOP_INDEX;
  uint16_t index_end = FORK_RIGHT_TOP_INDEX + FORK_TOP_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkLeftTop(uint32_t color) {
  uint16_t index_start = FORK_LEFT_TOP_INDEX;
  uint16_t index_end = FORK_LEFT_TOP_INDEX + FORK_TOP_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkRightSide(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_SIDE_INDEX;
  uint16_t index_end = FORK_RIGHT_SIDE_INDEX + FORK_SIDE_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkLeftSide(uint32_t color) {
  uint16_t index_start = FORK_LEFT_SIDE_INDEX;
  uint16_t index_end = FORK_LEFT_SIDE_INDEX + FORK_SIDE_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkRightBottom(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_BOTTOM_INDEX;
  uint16_t index_end = FORK_RIGHT_BOTTOM_INDEX + FORK_BOTTOM_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkLeftBottom(uint32_t color) {
  uint16_t index_start = FORK_LEFT_BOTTOM_INDEX;
  uint16_t index_end = FORK_LEFT_BOTTOM_INDEX + FORK_BOTTOM_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkRightFront(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_FRONT_INDEX;
  uint16_t index_end = FORK_RIGHT_FRONT_INDEX + FORK_FRONT_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}

void setForkLeftFront(uint32_t color) {
  uint16_t index_start = FORK_LEFT_FRONT_INDEX;
  uint16_t index_end = FORK_LEFT_FRONT_INDEX + FORK_FRONT_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
  frontforkstrip.show();
}


void setHandlebarCircleLeftPixel(uint16_t index, uint32_t color) {
  // Need to map left/right logic
}

void setHandlebarCircleRightPixel(uint16_t index, uint32_t color) {
  
}

//
//void setHandlebarCircle(uint32_t color) {
//  for(uint16_t i=0; i<handlebarcircle.numPixels(); i++) {
//    handlebarcircle.setPixelColor(i, color);
//  }
//
////  handlebarcircle.show();
//}

//void setHandlebarCirclePixel(uint16_t index, uint32_t color) {
//  handlebarcircle.setPixelColor(index, color);
//
//  handlebarcircle.show();
//}


void setHandlebarStrip(uint32_t color) {
  setHandlebarStripRight(color);
  setHandlebarStripLeft(color);
}

void setHandlebarStripRight(uint32_t color) {
  for(uint16_t i=0; i<16; i++) {
    handlebarstrip.setPixelColor(i, color);
  }
  handlebarstrip.show();
}

void setHandlebarStripLeft(uint32_t color) {
  for(uint16_t i=16; i<32; i++) {
    handlebarstrip.setPixelColor(i, color);
  }
  handlebarstrip.show();
}

void setHandlebarStripPixel(uint16_t index, uint32_t color) {
  setHandlebarStripRightPixel(index, color);
  setHandlebarStripLeftPixel(index, color);
}

void setHandlebarStripRightPixel(uint16_t index, uint32_t color) {
  handlebarstrip.setPixelColor(index, color);
  handlebarstrip.show();
}

void setHandlebarStripLeftPixel(uint16_t index, uint32_t color) {
  handlebarstrip.setPixelColor((31 - index), color);
  handlebarstrip.show();
}


void readSpectrum() {
  //reset the data
  digitalWrite(SPECTRUMSHIELD_PIN_RESET, HIGH);
  digitalWrite(SPECTRUMSHIELD_PIN_RESET, LOW);
  
  //loop thru all 7 bands
  for(int band=0; band < 7; band++) {
    digitalWrite(SPECTRUMSHIELD_PIN_STROBE,LOW); // go to the next band 
    delayMicroseconds(50); //gather some data
    left[band] = analogRead(SPECTRUMSHIELD_PIN_LEFT); // store left band reading
    right[band] = analogRead(SPECTRUMSHIELD_PIN_RIGHT); // store right band reading
    digitalWrite(SPECTRUMSHIELD_PIN_STROBE,HIGH); // reset the strobe pin
  }
}

int getNumLEDsFromValue(int powerValue, int numLedsInSegment) {
  return map(powerValue, MINPOWER, MAXPOWER, 0, numLedsInSegment);
}

int getBrightnessFromValue(int powerValue) {
  return map(powerValue, MINPOWER, MAXPOWER, 0, MAX_BRIGHTNESS);
}

uint32_t getColorFromValue(int powerValue, boolean isRGBW) {
  switch(mode) {
    case 0: default:

      break;
  }
}


void clearAll() {
  frontforkstrip.clear();
  handlebarstrip.clear();
//  handlebarcircle.clear();

}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<frontforkstrip.numPixels(); i++) {
    frontforkstrip.setPixelColor(i, c);
    frontforkstrip.show();
    delay(wait);
  }
}

// Fade off with reducing brightness - bad way to do this, setBrightness() is meant for setup
void fadeOff(uint16_t wait) {
  byte brightness;
  while ((brightness = frontforkstrip.getBrightness()) > 0) {
    frontforkstrip.setBrightness(--brightness);
    frontforkstrip.show(); // This sends the updated pixel brightness to the hardware.
    delay(wait); // Delay for a period of time (in milliseconds).
  }
}




