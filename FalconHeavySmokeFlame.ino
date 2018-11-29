// Falcon Heavy (or other rocket) animation of flames/smoke
//   This code drives an array of NeoPixels (WS2812B) installed in the 
//   center of a 3D Printed rocket flame and smoke plume.  
//
// Copyright 2018 Danal Estes all right reserved.  No part of this code may be reproduced, in whole or in part, by any means, without prior written permission from Danal Estes
// Released under Creative Commons - Non Commercial - Attribution license. 
// This code is made available "As Is". No warranty of merchantability or fitness for a particular puprose is expressed or implied. 

#include <Adafruit_NeoPixel.h>  //Adafruit Supplied.

#define STRIP0           2
#define STRIP1           3
#define STRIP2           4

#define LED             13    // Onboard LED (not NeoPixel) pin

// NEOPIXEL DEFINITIONS --------------------------------------------------------

#define PIXELSPERSTRIP 7
Adafruit_NeoPixel Strip0 = Adafruit_NeoPixel(PIXELSPERSTRIP, STRIP0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strip1 = Adafruit_NeoPixel(PIXELSPERSTRIP, STRIP1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strip2 = Adafruit_NeoPixel(PIXELSPERSTRIP, STRIP2, NEO_GRB + NEO_KHZ800);

// ANIMATION STUFF -------------------------------------------------------------

#define       AnimatePeriod 10L
unsigned long loopMillis;
unsigned long prevMillis = 0L;

uint32_t      pixelQueue[PIXELSPERSTRIP];
uint8_t       pFadeQueue[PIXELSPERSTRIP];
uint8_t       otherColor = 0;

// Setup          -------------------------------------------------------------

void setup() {
  pinMode(LED, OUTPUT);
  Strip0.begin();
  Strip1.begin();
  Strip2.begin();
}

// Loop           -------------------------------------------------------------

void loop() {
   loopMillis = millis();
   if (!((loopMillis - prevMillis) >= AnimatePeriod)) {return;}
   prevMillis = loopMillis;

    pixelQueue[PIXELSPERSTRIP-1] = Strip0.Color(255, 128, 000); 
    pFadeQueue[PIXELSPERSTRIP-1] = 1;
  if (random(1024) >  768) {
    pixelQueue[PIXELSPERSTRIP-1] = Strip0.Color(64, 032, 000);
    pFadeQueue[PIXELSPERSTRIP-1] = 1;
  } else if ((0==otherColor) && (random(1024) > 1000)) {
    pixelQueue[PIXELSPERSTRIP-1] = Strip0.Color(255, 255, 255);
    pFadeQueue[PIXELSPERSTRIP-1] = 0;
    flashAllPixels(5,Strip0.Color(255, 255, 255));
    otherColor = PIXELSPERSTRIP * 2;
  } else if ((0==otherColor) && (random(1024) > 1000)) {
    pixelQueue[PIXELSPERSTRIP-1] = Strip0.Color(255, 000, 000);
    pFadeQueue[PIXELSPERSTRIP-1] = 0;
    otherColor = PIXELSPERSTRIP * 2;
  }
 
  setAllPixels();   
  showAllStrips();
  advanceQueue();
  (0 == otherColor) ? otherColor : otherColor--;
}

// Service Subfuncitons  -------------------------------------------------------

void flashAllPixels(int ms, uint32_t color) {
  for(int i=PIXELSPERSTRIP; i >= 0; i--) {setPixelColorAllStrips(i, color);}
  showAllStrips();
  delay(ms);  
  prevMillis+=500;
}
/*void flashAllPixels(int ms, uint32_t color) {
  for(int i=0; i < PIXELSPERSTRIP; i++) {setPixelColorAllStrips(i, color);}
  showAllStrips();
  delay(ms);  
  prevMillis+=500;
}*/
void setAllPixels() {
  for(int i=0; i < PIXELSPERSTRIP; i++) {
    if (pFadeQueue[i]) {
      uint8_t r = ((i+1) * uint8_t(pixelQueue[i] >> 16)) / PIXELSPERSTRIP;
      uint8_t g = ((i+1) * uint8_t(pixelQueue[i] >>  8)) / PIXELSPERSTRIP;
      uint8_t b = ((i+1) * uint8_t(pixelQueue[i]      )) / PIXELSPERSTRIP;
      setPixelColorAllStrips(i, Strip0.Color(r, g, b));
    } else {
      setPixelColorAllStrips(i, pixelQueue[i]);
    }
  }
}

void setPixelColorAllStrips(int i, uint32_t color) {
  Strip0.setPixelColor(i, color);
  Strip1.setPixelColor(i, color);
  Strip2.setPixelColor(i, color);
}

void advanceQueue() {
    for(int i=0; i < PIXELSPERSTRIP - 1; i++) {
      pixelQueue[i] = pixelQueue[i+1];
      pFadeQueue[i] = pFadeQueue[i+1];
    }
}

void showAllStrips() {
  Strip0.show();
  Strip1.show();
  Strip2.show();
}


