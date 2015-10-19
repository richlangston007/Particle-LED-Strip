/*
 * Based on example particle neopixel application, which was ported from Adafruit
 * rich@thelangstons.org
 * 
 */

#include "application.h"
#include "neopixel/neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 120
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int powerButton = D4;
int brightButton = D5;
int colorButton = D6;
int photoresistor = A0;
//int analogvalue;
int pressed;
int buttonIndex=0;
uint32_t currentColor=0xffffff;
bool currentPower=0;
int currentBright=255;

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Particle.function("setColor",setColor);
  Particle.function("powerState",powerState);
  Particle.function("brightness",setLedBright);
//  pinMode(photoresistor,INPUT);
  pinMode(colorButton,INPUT);
  pinMode(powerButton,INPUT);
  pinMode(brightButton,INPUT);
//    Particle.variable("analogvalue", &analogvalue, INT);
}
void loop() 
{
  //rainbow(20);
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //analogvalue = analogRead(photoresistor);
  //if (analogvalue>2600) colorWipe(0x00,2); //if it's too bright, turn off the LEDs. 
  if (digitalRead(powerButton)) {
      if (currentPower) {
          colorWipe(0x00,2); 
          currentPower=0;
      } else {
          colorWipe(currentColor,2);
          currentPower=1;
      }
      }
      
  //if (pressed) colorWipe(0x00,2);
  if (digitalRead(colorButton)) colorPressed();
  if (digitalRead(brightButton)) {
      currentBright=currentBright-25;
      if (currentBright<0) currentBright=255;
      strip.setBrightness(currentBright);
      colorWipe(currentColor,1);
 //     strip.show();
      Particle.publish("bright button pressed");
//      unsigned char buffer;
//      memcpy( buffer, &currentBright, 4 );
      //Particle.publish(buffer);
  } 
}

int setLedBright(String brightness) {
    int bright = atoi(brightness);
//    Particle.publish(int bright);
    if ((bright<0) || (bright>100)) {
        Particle.publish("bright isn't a number?");
        bright=0;
    }
    bright=bright*2.5;
    //if (bright==100) bright=0; // 0 is max brightness, 1 is off, 255 is almost max.  see neopixel.ccp for docs
    strip.setBrightness(bright);
    strip.show();
    Particle.publish(brightness);
    //exit(0);
}
int powerState(String powerState) {
    int power = atoi(powerState);
  
      if (currentPower) {
          colorWipe(0x00,2); 
          currentPower=0;
      } else {
          colorWipe(currentColor,2);
          currentPower=1;
      }
      

}
void colorPressed() {
    uint32_t colors[] = { 0xffffff, 0xFF0000, 0x00ff00,0x0000ff, 0xff00ff,0xffff00};
    if (buttonIndex==6) buttonIndex=0;
    colorWipe(colors[buttonIndex],1);
    currentColor=colors[buttonIndex];
    buttonIndex++;
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

int setColor(String color){
    bool success;
    if (color == "red") {
        colorWipe(0xFF0000,2);
        currentColor=0xff0000;
    }
    if (color == "green") {
        colorWipe(0x00ff00,2);
        currentColor=0x00ff00;
    }
    if (color == "blue") {
        colorWipe(0x0000ff,2);
        currentColor=0x0000ff;
    }
    if (color == "purple") {
        colorWipe(0xff00ff,2);
        currentColor=0xff00ff;
    }
    if (color == "yellow") {
        colorWipe(0xffff00,2);
        currentColor=0xffff00;
    }
    if (color == "white") {
        colorWipe(0xffffff,2);
        currentColor=0xffffff;
    }
    if (color == "off") {
        colorWipe(0x00,2);   
        currentColor=0x00;
    }
    success = Particle.publish(color);
    if (!success) {
  // get here if event publish did not work
    }
    //exit(0);
}
