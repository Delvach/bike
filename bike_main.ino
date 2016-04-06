#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <gamma.h>
#include <Adafruit_NeoMatrix.h>


#define FRONT_STRIP_NEO_PTYPE  NEO_GRBW

#define FRONT_STRIP_PIN 8
#define FRONT_STRIP_NUM 98 

#define BACK_LIGHT_PIN 7

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








Adafruit_NeoPixel frontstrip = Adafruit_NeoPixel(FRONT_STRIP_NUM, FRONT_STRIP_PIN, FRONT_STRIP_NEO_PTYPE + NEO_KHZ800);

Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

uint32_t _RED = frontstrip.Color(64, 0, 0, 0);
uint32_t _GREEN = frontstrip.Color(0, 127, 0, 0);
uint32_t _BLACK = frontstrip.Color(0, 0, 0, 0);
uint32_t _PURPLE = frontstrip.Color(127, 0, 127, 0);
uint32_t _WHITE = frontstrip.Color(0, 0, 0, 127);

#define WAITTIME 350


void setup() {
  frontstrip.begin();
  frontstrip.setBrightness(MAX_BRIGHTNESS);
  frontstrip.show();
  
  backlightmatrix.begin();
  backlightmatrix.setBrightness(MAX_BRIGHTNESS);
  backlightmatrix.show();

  setForkRightBottom(_PURPLE);
  setForkLeftBottom(_PURPLE);
}

void loop() {

//  testForkAreas();

//  testForkScroll();

testFork(); 

  /*
  colorWipe(frontstrip.Color(150,0,0,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright green color.
  colorWipe(frontstrip.Color(0,150,0,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  
  colorWipe(frontstrip.Color(0,0,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright cyan color.
  colorWipe(frontstrip.Color(0,150,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value

  // Moderately bright purple color.
  colorWipe(frontstrip.Color(150,0,150,0), LWAIT);
  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS); // restore max brightness value


    colorWipe(frontstrip.Color(0,0,0,150), LWAIT);

  // Fade off with reducing brightness
  fadeOff(LWAIT);
  frontstrip.clear(); // turn all pixels off
  frontstrip.setBrightness(MAX_BRIGHTNESS);
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
  frontstrip.setPixelColor(trueIndex, color);
  frontstrip.show();
}

void setForkLeftFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_RIGHT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontstrip.setPixelColor(trueIndex, color);
  frontstrip.show();
}

void setForkRightSidePixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = index + FORK_LEFT_SIDE_INDEX;
  frontstrip.setPixelColor(trueIndex, color);
  frontstrip.show();
  
}

void setForkRightFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_LEFT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontstrip.setPixelColor(trueIndex, color);
  frontstrip.show();
  
}

#define TESTWAIT 5

void testFork() {
  for(uint16_t i = 0; i < 18; i++) {
    setForkFrontPixel(i, _GREEN);
    if(i > 0) setForkFrontPixel(i-1, _BLACK);
    delay(TESTWAIT);
  }
  uint16_t idx = 0;
  for(uint16_t y = 0; y < 18; y++) {
    idx = 17-y;
    setForkFrontPixel(idx, _GREEN);
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
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkLeftTop(uint32_t color) {
  uint16_t index_start = FORK_LEFT_TOP_INDEX;
  uint16_t index_end = FORK_LEFT_TOP_INDEX + FORK_TOP_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkRightSide(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_SIDE_INDEX;
  uint16_t index_end = FORK_RIGHT_SIDE_INDEX + FORK_SIDE_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkLeftSide(uint32_t color) {
  uint16_t index_start = FORK_LEFT_SIDE_INDEX;
  uint16_t index_end = FORK_LEFT_SIDE_INDEX + FORK_SIDE_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkRightBottom(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_BOTTOM_INDEX;
  uint16_t index_end = FORK_RIGHT_BOTTOM_INDEX + FORK_BOTTOM_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkLeftBottom(uint32_t color) {
  uint16_t index_start = FORK_LEFT_BOTTOM_INDEX;
  uint16_t index_end = FORK_LEFT_BOTTOM_INDEX + FORK_BOTTOM_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkRightFront(uint32_t color) {
  uint16_t index_start = FORK_RIGHT_FRONT_INDEX;
  uint16_t index_end = FORK_RIGHT_FRONT_INDEX + FORK_FRONT_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}

void setForkLeftFront(uint32_t color) {
  uint16_t index_start = FORK_LEFT_FRONT_INDEX;
  uint16_t index_end = FORK_LEFT_FRONT_INDEX + FORK_FRONT_LENGTH;
  
  for(uint16_t i = index_start; i < index_end; i++) {
    frontstrip.setPixelColor(i, color);
  }
  frontstrip.show();
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<frontstrip.numPixels(); i++) {
    frontstrip.setPixelColor(i, c);
    frontstrip.show();
    delay(wait);
  }
}

// Fade off with reducing brightness
void fadeOff(uint16_t wait) {
  byte brightness;
  while ((brightness = frontstrip.getBrightness()) > 0) {
    frontstrip.setBrightness(--brightness);
    frontstrip.show(); // This sends the updated pixel brightness to the hardware.
    delay(wait); // Delay for a period of time (in milliseconds).
  }
}
