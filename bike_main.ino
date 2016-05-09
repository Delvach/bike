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

#define MAX_BRIGHTNESS  32 // set max brightness


// prototype light mapping

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








Adafruit_NeoPixel frontforkstrip = Adafruit_NeoPixel(FRONT_STRIP_NUM, FORK_STRIP_PIN, WRGB_NEO_PTYPE + NEO_KHZ800);
Adafruit_NeoPixel handlebarstrip = Adafruit_NeoPixel(HANDLEBAR_STRIP_NUM, HANDLEBAR_STRIP_PIN, WRGB_NEO_PTYPE + NEO_KHZ800);
Adafruit_NeoPixel handlebarcircle = Adafruit_NeoPixel(HANDLEBAR_CIRCLE_NUM, HANDLEBAR_CIRCLE_PIN, RGB_NEO_PTYPE + NEO_KHZ800);

Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);



uint32_t _RED = frontforkstrip.Color(64, 0, 0, 0);
uint32_t _GREEN = frontforkstrip.Color(0, 127, 0, 0);
uint32_t _BLACK = frontforkstrip.Color(0, 0, 0, 0);
uint32_t _PURPLE = frontforkstrip.Color(127, 0, 127, 0);
uint32_t _WHITE = frontforkstrip.Color(0, 0, 0, 127);

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

  handlebarcircle.begin();
  handlebarcircle.setBrightness(MAX_BRIGHTNESS);
  handlebarcircle.show();

  handlebarstrip.begin();
  handlebarstrip.setBrightness(MAX_BRIGHTNESS);
  handlebarstrip.show();

  setForkRightBottom(_PURPLE);
  setForkLeftBottom(_PURPLE);

  setHandlebarStrip(_PURPLE);

  setHandlebarCircle(handlebarcircle.Color(127, 0, 127));

  timeCurrent = millis();


  FrontFork_Animation.start(1);
}

void loop() {

  

  unsigned long timeDiff = millis() - timeCurrent;
  timeCurrent = millis();

//  testForkAreas();

//  testForkScroll();

testFork();



   if(FrontFork_Animation.isActive() && FrontFork_Animation.execute()) {
    
   }

  /*
  colorWipe(frontforkstrip.Color(150,0,0,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright green color.
  colorWipe(frontforkstrip.Color(0,150,0,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  
  colorWipe(frontforkstrip.Color(0,0,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright cyan color.
  colorWipe(frontforkstrip.Color(0,150,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright purple color.
  colorWipe(frontforkstrip.Color(150,0,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value


    colorWipe(frontforkstrip.Color(0,0,0,150), LWAIT);

  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontforkstrip.clear(); // turn all pixels off
  frontforkstrip.setBrightness(MAX_BRIGHTNESS);
  */

  
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


void setHandlebarCircle(uint32_t color) {
  for(uint16_t i=0; i<handlebarcircle.numPixels(); i++) {
    handlebarcircle.setPixelColor(i, color);
  }

  handlebarcircle.show();
}

void setHandlebarCirclePixel(uint16_t index, uint32_t color) {
  handlebarcircle.setPixelColor(index, color);

  handlebarcircle.show();
}


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


// handlebarstrip

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<frontforkstrip.numPixels(); i++) {
    frontforkstrip.setPixelColor(i, c);
    frontforkstrip.show();
    delay(wait);
  }
}

// Fade off with reducing brightness
void fadeOff(uint16_t wait) {
  byte brightness;
  while ((brightness = frontforkstrip.getBrightness()) > 0) {
    frontforkstrip.setBrightness(--brightness);
    frontforkstrip.show(); // This sends the updated pixel brightness to the hardware.
    delay(wait); // Delay for a period of time (in milliseconds).
  }
}
