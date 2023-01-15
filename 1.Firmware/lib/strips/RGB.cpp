#include "RGB.h"
// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    38
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8

unsigned char brightness = 30;
/*
Declare our NeoPixel strip object:
Argument 1 = Number of pixels in NeoPixel strip
Argument 2 = Arduino pin number (most are valid)
Argument 3 = Pixel type flags, add together as needed:
  NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
  NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
*/
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
int           pixelRoll = 0;           // Pattern Pixel Roll
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels
int rgb_flag = 0;
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void role_fill()
{
    int i= 0;
    pixelInterval = 30; 
    strip.fill(strip.Color(0, 0, 0));
    for(i = 0;i<pixelRoll;i++) 
    strip.setPixelColor(i, strip.Color(0,50,0)); 
    pixelRoll++;
    strip.show();
}
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void colorWipe(uint32_t color) {
    strip.setPixelColor(pixelCurrent, color); //  Set pixel's color (in RAM)
    strip.show();                             //  Update strip to match
    pixelCurrent++;                           //  Advance current pixel
    if(pixelCurrent >= pixelNumber)           //  Loop the pattern from the first LED
        pixelCurrent = 0;
}
// Fill the dots one after the other with a color
void colorWipe_delay(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}
void strip1() {
  pixelInterval = 100;   
  int i = 0;
  int led,spacing;
  led = LED_COUNT/4;
  spacing = LED_COUNT/2;
  strip.fill(strip.Color(0, 0, 0));
  for (i;i<led;i++)
  {
        strip.setPixelColor((i+pixelNumber-pixelCycle)%pixelNumber, Wheel((i*spacing + pixelQueue) & 255)); 
        strip.setPixelColor((i+pixelNumber-pixelCycle+11)%pixelNumber, Wheel((i*spacing + pixelQueue+128) & 255)); 
  }
  strip.show();
  pixelCycle++;
  if(pixelCycle>=pixelNumber)
        pixelCycle = 0;
  pixelQueue++;                                 //  Advance current cycle
  if(pixelQueue >= 256)
        pixelQueue = 0;                         //  Loop the cycle back to the begining
    
}
void strip2() {
    int i = 0;
    int led,spacing,opp;
    led = LED_COUNT/4;
    spacing = LED_COUNT/3;
    opp = LED_COUNT/2;
    strip.fill(strip.Color(0, 0, 0));
    for(i;i<led;i++)
    {
        int j =(led-i)%spacing*(30)+3;
        strip.setPixelColor((i+pixelNumber-pixelCycle)%pixelNumber, strip.Color(j,j,j)); 
        strip.setPixelColor((i+pixelNumber-pixelCycle+opp)%pixelNumber, strip.Color(j,j,j)); 
    }
    strip.show();
    pixelCycle++;
    if(pixelCycle>=pixelNumber)
        pixelCycle = 0;
}
void strip3() {
    int i = 0;
    strip.fill(strip.Color(0, 0, 0));
    int led,spacing,opp;
    led = LED_COUNT/4;
    spacing = LED_COUNT/3;
    opp = LED_COUNT/2;
    for(i;i<led;i++)
    {
        int j =i%spacing*(30)+3;
        strip.setPixelColor((i+pixelCycle)%pixelNumber, strip.Color(j,j,j)); 
        strip.setPixelColor((i+pixelCycle+opp)%pixelNumber, strip.Color(j,j,j)); 
    }
    strip.show();
    pixelCycle++;
    if(pixelCycle>=pixelNumber)
        pixelCycle = 0;
}
void rainbow1() {          
    pixelInterval = 30;   
    for(uint16_t i=0; i < pixelNumber; i++) {
        strip.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time  
    }
    strip.show();                             //  Update strip to match
    pixelCycle++;                             //  Advance current cycle
    if(pixelCycle >= 256)
        pixelCycle = 0;                         //  Loop the cycle back to the begining
}
void rainbow2()
{
  pixelInterval = 30;
    pixelCycle +=256; 
    strip.rainbow(pixelCycle);
    strip.show(); // Update strip with new contents
    if(pixelCycle >= 5*65536)
      pixelCycle = 0;
}
void pulse_rainbow1()
{
    pixelInterval = 30;
    pixelQueue+=1;
    if(pixelQueue>=brightness*2)
        pixelQueue = 0;
    if(pixelQueue<brightness)
        strip.setBrightness(pixelQueue); // Set BRIGHTNESS to about 1/5 (max = 255)
    else
        strip.setBrightness(brightness+brightness-pixelQueue); // Set BRIGHTNESS to about 1/5 (max = 255)
    pixelCycle +=256; 
    strip.rainbow(pixelCycle);
    strip.show(); // Update strip with new contents
    if(pixelCycle >= 5*65536)
        pixelCycle = 0;
}
void rgb_off(void)
{
    pixelInterval = 100;
    strip.fill(strip.Color(0, 0, 0));
    strip.show(); // Update strip with new contents
}

void strip_init(void) {
    strip.begin();                    // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();                     // Turn OFF all pixels ASAP
    strip.setBrightness(brightness);  // Set BRIGHTNESS to about 1/5 (max = 255)
}


void strip_start(unsigned long currentMillis){
    if (currentMillis - pixelPrevious >= pixelInterval) {  //  Check for expired time
      pixelPrevious = currentMillis;                       //  Run current frame
      rainbow1();
      switch (rgb_flag) {
        case 0:
          break;
        case 1:
          role_fill();
          break;
        case 2:
          rainbow2();
          break;
      }
  }
}