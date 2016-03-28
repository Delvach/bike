#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <gamma.h>
#include <Adafruit_NeoMatrix.h>



#define FRONT_STRIP_PIN 8
#define FRONT_STRIP_NUM 60

#define BACK_LIGHT_PIN 7


Adafruit_NeoPixel frontstrip = Adafruit_NeoPixel(FRONT_STRIP_NUM, FRONT_STRIP_PIN, NEO_WGRB + NEO_KHZ800);

Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);




void setup() {
  frontstrip.begin();
  backlightmatrix.begin();

}

void loop() {
  

}
