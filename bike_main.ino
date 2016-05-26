#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <AltSoftSerial.h>
#include <wavTrigger.h>
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
#define HANDLEBAR_CIRCLE_PIN 12
#define HANDLEBAR_CIRCLE_NUM 16

// Neopixels - handlebar upper (rgbw strip)
#define HANDLEBAR_STRIP_PIN 10
#define HANDLEBAR_STRIP_NUM 36

#define MEGAMATRIX_PIN 11



#define LWAIT    50


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
Adafruit_NeoPixel handlebarcircle = Adafruit_NeoPixel(HANDLEBAR_CIRCLE_NUM, HANDLEBAR_CIRCLE_PIN, RGB_NEO_PTYPE + NEO_KHZ800);

Adafruit_NeoMatrix backlightmatrix = Adafruit_NeoMatrix(8, 8, BACK_LIGHT_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix megaMatrix = Adafruit_NeoMatrix(32, 8, MEGAMATRIX_PIN,  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

boolean useWavTrigger = true;


wavTrigger wTrig;


int colStr = 191;

uint32_t wheelColorsRGBW[16];

/* COLOR DEFINITIONS */
const uint16_t rgb_colors[] = {
  megaMatrix.Color(colStr, 0, 0),
  megaMatrix.Color(colStr, colStr, 0),
  megaMatrix.Color(colStr, 0, colStr),
  megaMatrix.Color(0, colStr, 0),
  megaMatrix.Color(0, 0, colStr),
  megaMatrix.Color(0, colStr, colStr),
  
  megaMatrix.Color(colStr,0,  colStr/2),
  megaMatrix.Color(colStr, colStr, colStr)
  };

const uint16_t RGB_RED = megaMatrix.Color(colStr, 0, 0);
const uint16_t RGB_GREEN = megaMatrix.Color(0, colStr, 0);
const uint16_t RGB_BLUE = megaMatrix.Color(0, 0, colStr);
const uint16_t RGB_PURPLE = megaMatrix.Color(colStr, colStr, 0);
const uint16_t RGB_BLACK = megaMatrix.Color(0, 0, 0);
const uint16_t RGB_WHITE = megaMatrix.Color(colStr, colStr, colStr);






/* Options that will eventually be controlled via touchscreen and/or physical controls */
boolean use_audio = true;
int msDelaySpectrumUpdate = 5;
boolean cycleColors = false;


int MAX_BRIGHTNESS = 64; // set max brightness
int MINPOWER = 128;
int MAXPOWER = 1023;

/*
 * 0 - default
 * 
 */
int mode = 0;

int COLOR_STRENGTH = 128;
int RGBW_STRENGTH = 64;
uint32_t _RED = frontforkstrip.Color(COLOR_STRENGTH/2, 0, 0, 0);
uint32_t _GREEN = frontforkstrip.Color(0, COLOR_STRENGTH, 0, 0);
uint32_t _BLACK = frontforkstrip.Color(0, 0, 0, 0);
uint32_t _PURPLE = frontforkstrip.Color(COLOR_STRENGTH, 0, COLOR_STRENGTH, 0);
uint32_t _WHITE = frontforkstrip.Color(0, 0, 0, COLOR_STRENGTH);

const uint32_t rgbw_blue = frontforkstrip.Color(0, 0, COLOR_STRENGTH, 0);
const uint32_t rgbw_white = frontforkstrip.Color(0, 0, 0, RGBW_STRENGTH);



#define TEXT_SCROLL_DELAY 25

unsigned long timeCurrent;
unsigned long timeLastCircleEvent = 0;
unsigned long timeBetweenCircleEvents = 1000;
const long maxTotalSpectrumValue = 14322;

//Animation FrontFork_Animation = Animation(18, 20);

boolean useMegaMatrix = true;
boolean useFrontCircle = true;
boolean useBacklightMatrix = false;


// HSV library imports
int ihue = 0;        //-HUE (0-360)
int ibright = 0;     //-BRIGHTNESS (0-255)
int isat = 0;        //-SATURATION (0-255)

int ledsX[256][3];

uint32_t frontCircleWheelColors[16];
int cycleFrontCircleRainbowPosition = 0;

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial2.begin(9600);
  delay(50);

  if(useWavTrigger) {
    wTrig.start();
    delay(50);
//    wTrig.masterGain(-12);
    delay(50);
    wTrig.stopAllTracks();
    delay(10);
  }
  
  frontforkstrip.begin();
  frontforkstrip.setBrightness(MAX_BRIGHTNESS);
  frontforkstrip.show();

  if(useBacklightMatrix) {
    backlightmatrix.begin();
    backlightmatrix.setBrightness(MAX_BRIGHTNESS);
    backlightmatrix.show();
  }

  if(useFrontCircle) {
    handlebarcircle.begin();
    handlebarcircle.setBrightness(MAX_BRIGHTNESS);
    handlebarcircle.show();
  }
  
  handlebarstrip.begin();
  handlebarstrip.setBrightness(MAX_BRIGHTNESS);
  handlebarstrip.show();

  if(useMegaMatrix) {
    megaMatrix.begin();
    megaMatrix.setTextWrap(false);
    megaMatrix.setBrightness(MAX_BRIGHTNESS);
    megaMatrix.setTextColor(rgb_colors[0]);
    megaMatrix.show();
  }

  //initialize eq
  pinMode(SPECTRUMSHIELD_PIN_RESET, OUTPUT); // reset
  pinMode(SPECTRUMSHIELD_PIN_STROBE, OUTPUT); // strobe
  digitalWrite(SPECTRUMSHIELD_PIN_RESET,LOW); // reset low
  digitalWrite(SPECTRUMSHIELD_PIN_STROBE,HIGH); //pin 5 is RESET on the shield

  setForkRightBottom(frontforkstrip.Color(0, 0, 0, 255));
  setForkLeftBottom(frontforkstrip.Color(0, 0, 0, 255));

//  setHandlebarStrip(_PURPLE);
  if(false && useFrontCircle) {
    setHandlebarCircleAllPixels(handlebarcircle.Color(COLOR_STRENGTH, 0, COLOR_STRENGTH));
  }

  timeCurrent = millis();


  

//  FrontFork_Animation.start(1);
  initializeColorWheel();

  uint32_t tempColor;
  for(int index = 0; index < FORK_TOP_LENGTH; index++) {
//    tempColor = wheelColorsRGBW[map(index, 0, FORK_TOP_LENGTH-1, 0, 15)];
    tempColor = frontforkstrip.Color(64, 32, 0, 0);
    setForkLeftTopPixel(index, tempColor);
    setForkRightTopPixel(index, tempColor);
    
  }
//    if(index < leftValue) {
//      leftColor = _GREEN;
//    } else {
//      leftColor = _BLACK;
//    }
//    if(index < rightValue) {
//      rightColor = _GREEN;
//    } else {
//      rightColor = _BLACK;
//    }
//    setForkLeftTopPixel(index, leftColor);
//    setForkRightTopPixel(index, rightColor);
//  }
Serial.println("ready");
delay(50);

megaMatrix.drawPixel(0, 0, megaMatrix.Color(64, 0, 0));
megaMatrix.show();
delay(2000);
wTrig.trackPlaySolo(1);

}

boolean notyet = true;

int x    = megaMatrix.width();
int pass = 0;

void loop() {



  if(Serial2.available() > 0) {
      int command = Serial2.readStringUntil(';').toInt();
      int value = Serial2.readStringUntil('\n').toInt();
Serial.println(command);
Serial.println(value);      

      switch(command) {
        case 0:
          Serial.println("Setting max brightness");
          Serial.println(value);
          MAX_BRIGHTNESS = value;
          COLOR_STRENGTH = value;
          RGBW_STRENGTH = value;
          megaMatrix.setBrightness(MAX_BRIGHTNESS);
          frontforkstrip.setBrightness(MAX_BRIGHTNESS);
          backlightmatrix.setBrightness(MAX_BRIGHTNESS);
          handlebarcircle.setBrightness(MAX_BRIGHTNESS);
          handlebarstrip.setBrightness(MAX_BRIGHTNESS);
          
          break;
        case 1:
          Serial.println("Setting min audio level");
          Serial.println(value);
          MINPOWER = value;
          break;
        case 2:
          Serial.println("Setting max audio level");
          Serial.println(value);
          MAXPOWER = value;
          break;
        case 3:
          Serial.println("Setting mode");
          Serial.println(value);
          mode = value;
          break;
        case 4:
          Serial.println("Action");
          Serial.println(value);
          takeActionByIndex(value);
          break;
        case 5:
          Serial.println("Turbine Action");
          Serial.println(value);
          turbineAction(value);
          break;
      }
  }

  
  if(use_audio) readSpectrum();

  switch(mode) {
     case 0:
        if(use_audio) {
//          Serial.println(left[3]);
          updateMegaMatrixWithAudio();
          updateTopForksWithAudio();
          updateFrontForksWithAudio();
          delay(msDelaySpectrumUpdate);
        }
        break;



      case 1:
        megaMatrix.fillScreen(0);
        megaMatrix.setCursor(x, 0);
        megaMatrix.print(F("Happy Thursday"));
        if(--x < -96) {
          x = megaMatrix.width();
          if(++pass >= 3) pass = 0;
          megaMatrix.setTextColor(rgb_colors[pass]);
        }
        megaMatrix.show();
        delay(TEXT_SCROLL_DELAY);

        break;

      case 2:
        sparkle();
        break;

      case 3:
        testMatrix();
        break;

      case 4:
        randomFill();
        break;

      case 5:
        
        break;

      case 6:

        break;
  }

  unsigned long timeDiff = millis() - timeCurrent;
  timeCurrent = millis();

  if((millis() - timeLastCircleEvent) >= timeBetweenCircleEvents || true) {
    
    // Trigger circle event
    cycleFrontCircleRainbow();
    

    // Recalculate time until next event
    int currentSpectrumValue;
    for(byte i = 0; i < 7; i++) {
      currentSpectrumValue += left[i];
      currentSpectrumValue += right[i];
    }
    timeBetweenCircleEvents = map(currentSpectrumValue, 0, maxTotalSpectrumValue, 60000, 10);
    timeLastCircleEvent = millis();
  }
  
  
  
  frontforkstrip.show();
  handlebarstrip.show();
  
  
}

void takeActionByIndex(int index) {
  
}

void turbineAction(int index) {
  
}


/**************************************************************
 * 
 *  Spectrum Analyzer & LED integration 
 *  v0.1 - Basic audio integration
 * 
 *************************************************************/

void testMatrix() {
  int x;
  int y;
  for(x = 0; x < 32; x++) {
    megaMatrix.fillScreen(megaMatrix.Color(0, 0, 0));
    for(y = 0; y < 8; y++) {
      megaMatrix.drawPixel(x, y, megaMatrix.Color(0, 0, 255));
    }
    
    
//    megaMatrix.drawPixel(0, 0, megaMatrix.Color(x, 0, 0));
    megaMatrix.show();
    delay(20);
  }
  
}

void randomFill() {
  int x, y, c;
  x = random(0, 32);
  y = random(0, 8);
  c = random(0, 7);
  megaMatrix.drawPixel(x, y, rgb_colors[c]);
  megaMatrix.show();
}

void sparkle() {
  int x;
  int y;
  int c;
  for(x = 0; x < 32; x++) {
    for(y = 0; y < 8; y++) {
      c = random(0, 7);
      megaMatrix.drawPixel(x, y, rgb_colors[c]);
    }
    
    
//    megaMatrix.drawPixel(0, 0, megaMatrix.Color(x, 0, 0));
    
    
  }
  megaMatrix.show();
}






/**************************************************************
 * 
 *  Spectrum Analyzer & LED integration 
 *  v0.2 - Audio to MegaMatrix
 *  
 * 
 *  
 * 
 *************************************************************/

void updateTopForksWithAudio() {
  int leftValue, rightValue, index;
  uint32_t leftColor, rightColor;


  leftValue = map(left[2], MINPOWER, MAXPOWER, 0, FORK_TOP_LENGTH);
  rightValue = map(right[2], MINPOWER, MAXPOWER, 0, FORK_TOP_LENGTH);
  

  for(index = 0; index < FORK_TOP_LENGTH; index++) {
    if(index < leftValue) {
      leftColor = _GREEN;
    } else {
      leftColor = _BLACK;
    }
    if(index < rightValue) {
      rightColor = _GREEN;
    } else {
      rightColor = _BLACK;
    }
    setForkLeftTopPixel(index, leftColor);
    setForkRightTopPixel(index, rightColor);
  }


  
}

void updateFrontForksWithAudio() {
  int leftValue, rightValue, index;
  uint32_t leftColor, rightColor;

  leftValue = map(left[2], 0, 1023, 0, FORK_FRONT_LENGTH);
  rightValue = map(right[2], 0, 1023, 0, FORK_FRONT_LENGTH);
  
  for(index = 0; index < FORK_FRONT_LENGTH; index++) {
    leftColor = index < leftValue ? _PURPLE : _BLACK;
    rightColor = index < rightValue ? _PURPLE : _BLACK;
    setForkLeftFrontPixel(FORK_FRONT_LENGTH-index, leftColor);
    setForkRightFrontPixel(FORK_FRONT_LENGTH-index, rightColor);
  }

  leftValue = map(left[4], 0, 1023, 0, FORK_SIDE_LENGTH);
  rightValue = map(right[4], 0, 1023, 0, FORK_SIDE_LENGTH);

  for(index = 0; index < FORK_SIDE_LENGTH; index++) {
    leftColor = index < leftValue ? _RED : _BLACK;
    rightColor = index < rightValue ? _RED : _BLACK;
    setForkLeftSidePixel(FORK_SIDE_LENGTH-index, leftColor);
    setForkRightSidePixel(FORK_SIDE_LENGTH-index, rightColor);
  }


  leftValue = map(left[5], MINPOWER, MAXPOWER, 0, HANDLEBAR_LENGTH);
  rightValue = map(right[5], MINPOWER, MAXPOWER, 0, HANDLEBAR_LENGTH);
  
  for(index = 0; index < HANDLEBAR_LENGTH; index++) {
    leftColor = index < leftValue ? rgbw_blue : _BLACK;
    rightColor = index < rightValue ? rgbw_blue : _BLACK;
    setHandlebarStripLeftPixel(index, leftColor);
    setHandlebarStripRightPixel(index, rightColor);
  }

  


  
}







void updateMegaMatrixWithAudio() {
  megaMatrix.fillScreen(megaMatrix.Color(0, 0, 0));
  int column = 0;
  int x = 0;
  int channelStr;

  boolean bassInside = false;

  for(x = 0; x < 7; x++) {
    channelStr = map(left[x], MINPOWER, MAXPOWER, 0, 9);
//    setMegaMatrixColumnStr(bassInside ? 7-column++ : column++, channelStr, getColorByBandAndStrength(x, channelStr));
//    setMegaMatrixColumnStr(bassInside ? 7-column++ : column++, channelStr, getColorByBandAndStrength(x, channelStr));
    setMegaMatrixColumnHSV(column++, channelStr, x);
    setMegaMatrixColumnHSV(column++, channelStr, x);
  }

  column++;column++;column++;column++;

  for(x = 0; x < 7; x++) {
    channelStr = map(right[7-x], MINPOWER, MAXPOWER, 0, 9);
//    setMegaMatrixColumnStr(column++, channelStr, getColorByBandAndStrength(6-x, channelStr));
//    setMegaMatrixColumnStr(column++, channelStr, getColorByBandAndStrength(6-x, channelStr));
    setMegaMatrixColumnHSV(column++, channelStr, 6-x);
    setMegaMatrixColumnHSV(column++, channelStr, 6-x);
  }

  megaMatrix.show();
  if(cycleColors) int off = getOffset(true);
  
}

void setMegaMatrixColumnHSV(int column, int str, int band) {
  int row;
  uint16_t color;
  for(row = 0; row < 8; row++) {
    color = getColorByColumnRowStrength(band, row, str);
    megaMatrix.drawPixel(column, 7-row, row < str ? color : RGB_BLACK);
  }
//  megaMatrix.show();
  
}

int offset = 0;

int getOffset(boolean updateThis) {
  if(updateThis) {
    if(offset == 359) {
      offset = 0;
    } else {
      offset++;
    }
  }
  return offset;
}

uint16_t getColorByColumnRowStrength(int column, int row, int strength) {
  int thisColor[3];

  int ihue = map(column, 0, 6, 0, 300);
  int isat = map(strength, 0, 9, 0, 255);
//  int ival = map(row, 0, 7, 240, 0); // probably reversed
  int newHue = ihue;
  
  if(cycleColors) {
    int off = getOffset(false);
    int newHue = ihue + off;
    if(newHue > 259) {
      newHue = 360-newHue;
    }
  }

  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  HSVtoRGB(newHue, 255, 255, thisColor);
  return megaMatrix.Color(thisColor[0],thisColor[1],thisColor[2]);

  
}



uint16_t getColorByBandAndStrength(int band, int strength) {
  int thisColor[3];

  int ihue = map(band, 0, 6, 0, 300);
  int isat = map(strength, 0, 9, 0, 255);
//  int ival = map(row, 0, 

  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  HSVtoRGB(ihue, 255, isat, thisColor);
  return megaMatrix.Color(thisColor[0],thisColor[1],thisColor[2]);

  
}

uint16_t getColorByBand(int band) {
  return rgb_colors[band];
}




void setMegaMatrixColumnStr(int column, int str, uint16_t color) {
  int y;
  
  for(y = 0; y < 8; y++) {
    megaMatrix.drawPixel(column, 7-y, y < str ? color : RGB_BLACK);
  }
  
}




/**************************************************************
 * 
 *  Front Fork
 * 
 *************************************************************/

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
//  frontforkstrip.show();
}

void setForkLeftFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_RIGHT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontforkstrip.setPixelColor(trueIndex, color);
//  frontforkstrip.show();
}

void setForkRightSidePixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = index + FORK_LEFT_SIDE_INDEX;
  frontforkstrip.setPixelColor(trueIndex, color);
//  frontforkstrip.show();
  
}

void setForkRightFrontPixel(uint16_t index, uint32_t color) {
  uint16_t trueIndex = (FORK_LEFT_FRONT_INDEX + (FORK_FRONT_LENGTH - 1)) - index;
  frontforkstrip.setPixelColor(trueIndex, color);
//  frontforkstrip.show();
  
}




void setForkLeftTopPixel(int displayIndex, uint32_t color) {
  int index_start = FORK_LEFT_TOP_INDEX;
  int index_end = FORK_LEFT_TOP_INDEX + FORK_TOP_LENGTH;

  for(int i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
//  frontforkstrip.show();
}

void setForkRightTopPixel(uint16_t displayIndex, uint32_t color) {
  uint16_t index_start = FORK_RIGHT_TOP_INDEX;
  uint16_t index_end = FORK_RIGHT_TOP_INDEX + FORK_TOP_LENGTH;

  for(uint16_t i = index_start; i < index_end; i++) {
    frontforkstrip.setPixelColor(i, color);
  }
//  frontforkstrip.show();
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




/**************************************************************
 * 
 *  Handlebar Front Circle
 * 
 *************************************************************/

void setHandlebarCircleLeftPixel(uint16_t index, uint32_t color) {
  // Need to map left/right logic
}

void setHandlebarCircleRightPixel(uint16_t index, uint32_t color) {
  
}


void setHandlebarCircleAllPixels(uint32_t color) {
  for(uint16_t i=0; i<handlebarcircle.numPixels(); i++) {
    handlebarcircle.setPixelColor(i, color);
  }

  handlebarcircle.show();
}



void initializeColorWheel() {
  
  int thisColor[3];
  
  for(int i=0; i< 16; i++) {
    HSVtoRGB(map(i, 0, 15, 0, 240), 255, 255, thisColor);
    frontCircleWheelColors[i] = handlebarcircle.Color(thisColor[0],thisColor[1],thisColor[2]);
    wheelColorsRGBW[i] = frontforkstrip.Color(thisColor[0],thisColor[1],thisColor[2], 0);
  }
}


void cycleFrontCircleRainbow() {
  
  int colorWheelIndex;
  for(uint16_t i=0; i<16; i++) {
    colorWheelIndex = i + cycleFrontCircleRainbowPosition;
    if(colorWheelIndex > 15) colorWheelIndex = colorWheelIndex-15;
    
    handlebarcircle.setPixelColor(i, frontCircleWheelColors[colorWheelIndex]);
  }

  handlebarcircle.show();

  cycleFrontCircleRainbowPosition++;
  if(cycleFrontCircleRainbowPosition > 15) cycleFrontCircleRainbowPosition = 0;
}

void setHandlebarCirclePixel(uint16_t index, uint32_t color) {
  handlebarcircle.setPixelColor(index, color);

  handlebarcircle.show();
}

/**************************************************************
 * 
 *  Handlebar Front Strips
 * 
 *************************************************************/

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

}

void setHandlebarStripLeftPixel(uint16_t index, uint32_t color) {
  handlebarstrip.setPixelColor((31 - index), color);

}


/**************************************************************
 * 
 *  Audio Utility
 * 
 *************************************************************/

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

/**************************************************************
 * 
 *  NeoPixel Utility
 * 
 *************************************************************/

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return megaMatrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return megaMatrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return megaMatrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void clearAll() {
  frontforkstrip.clear();
  handlebarstrip.clear();
  if(useFrontCircle) {
    handlebarcircle.clear();
  }

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



void rainbow_fade(int idelay) { //-FADE ALL LEDS THROUGH HSV RAINBOW
    ihue++;
    if (ihue >= 359) {ihue = 0;}
    int thisColor[3];
    HSVtoRGB(ihue, 255, 255, thisColor);
    for(int idex = 0 ; idex < megaMatrix.numPixels(); idex++ ) {
      megaMatrix.setPixelColor(idex,thisColor[0],thisColor[1],thisColor[2]); 
    }
    megaMatrix.show();    
    delay(idelay);
}


//void flame(int topIndex) {
//  int acolor[3];
//  int idelay = random(0,35);
//  
//  float hmin = 0.1; float hmax = 45.0;
//  float hdif = hmax-hmin;
//  int randtemp = random(0,3);
//  float hinc = (hdif/float(topIndex))+randtemp;
//
//  int ahue = hmin;
//  for(int i = 0; i < topIndex; i++ ) {
//    
//    ahue = ahue + hinc;
//
//    HSVtoRGB(ahue, 255, 255, acolor);
//    
//    leds[i].r = acolor[0]; leds[i].g = acolor[1]; leds[i].b = acolor[2];
//    int ih = horizontal_index(i);
//    leds[ih].r = acolor[0]; leds[ih].g = acolor[1]; leds[ih].b = acolor[2];
//    
//    leds[TOP_INDEX].r = 255; leds[TOP_INDEX].g = 255; leds[TOP_INDEX].b = 255;
//  
//    megaMatrix.show();
//    delay(idelay);
//  }
//}


void HSVtoRGB(int hue, int sat, int val, int colors[3]) {
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r, g, b, base;

  if (sat == 0) { // Achromatic color (gray).
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;
  } else  {
    base = ((255 - sat) * val)>>8;
    switch(hue/60) {
      case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
        break;
      case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
        break;
      case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
        break;
      case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
        break;
      case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
        break;
      case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
        break;
    }
    colors[0]=r;
    colors[1]=g;
    colors[2]=b;
  }
}



