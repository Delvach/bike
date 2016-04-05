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

Adafruit_NeoPixel frontstrip = Adafruit_NeoPixel(FRONT_STRIP_NUM, FRONT_STRIP_PIN, FRONT_STRIP_NEO_PTYPE + NEO_KHZ800);

Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);




void setup() {
  frontstrip.begin();
  frontstrip.setBrightness(MAX_BRIGHTNESS);
  frontstrip.show();
  
  backlightmatrix.begin();
  backlightmatrix.setBrightness(MAX_BRIGHTNESS);
  backlightmatrix.show();
}

void loop() {

  
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
