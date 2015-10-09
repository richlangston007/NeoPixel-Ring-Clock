// This #include statement was automatically added by the Particle IDE.

/*
 * Based on example particle neopixel application, which was ported from Adafruit
 * rich@thelangstons.org
 *
 */

#include "application.h"
#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
Adafruit_NeoPixel minutes = Adafruit_NeoPixel(60, D2, WS2812B);
Adafruit_NeoPixel hours = Adafruit_NeoPixel(24, D3, WS2812B);

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
int timeHours=0;
uint32_t hourColor=0xffff00;
int timeMinutes=0;
int timeSeconds=0;
int hourLED=0;

void setup()
{
  minutes.begin();
  minutes.show(); // Initialize all pixels to 'off'
  hours.begin();
  hours.show(); // Initialize all pixels to 'off'
  //pinMode(photoresistor, Input);
  Serial.begin(9600);
  Time.zone(-8); //PST
}
void loop()
{
  timeHours = Time.hourFormat12();
  timeMinutes=Time.minute();
  timeSeconds=Time.second();
//  if (timeSeconds==0) {
    int light=analogRead(photoresistor);
    Serial.print("photo:");
    Serial.println(light);
  if (light<2500) {
      hours.setBrightness(120);
      hours.show();
      minutes.setBrightness(120);
      minutes.show();
    }
  //  Particle.publish(light);
  //}
  //Particle.publish(timeHours);
  //colorWipe(0xff0000,20);
  if (timeSeconds==0) minutes.setPixelColor(59,0);

  minutes.setPixelColor(timeMinutes-2,0x00);
  minutes.setPixelColor(timeMinutes-1,0x333333);
  minutes.setPixelColor(timeMinutes,0x888888);
  minutes.setPixelColor(timeMinutes+1,0x333333);
  if (timeSeconds!=0) minutes.setPixelColor(timeSeconds-1,0x00);
  minutes.setPixelColor(timeSeconds,0x008800);
  minutes.show();


    hourLED=timeHours*2;
  if (timeHours==12) {
    hours.setPixelColor(1,hourColor);
    hours.setPixelColor(0,hourColor);
    hours.setPixelColor(23,0);
    hours.setPixelColor(22,0);
  } else {
    //hours.setPixelColor(23,0);
    hours.setPixelColor(hourLED,0xffff00);
    hours.setPixelColor(hourLED+1,0xffff00);
    hours.setPixelColor(hourLED-1,0);
    hours.setPixelColor(hourLED-2,0);
    //Serial.print("I am here");
  //  hours.setPixelColor(9,0xff1111);
  //  hours.setPixelColor(18,0xff1111);
//    if (timeHours==0) hours.setPixelColor(23,0);
    }
//    Serial.print("time:");
//    Serial.println(hourLED);

  hours.show();

}

int setLedBright(String brightness) {
    int bright = atoi(brightness);
//    Particle.publish(int bright);
    if ((bright<0) || (bright>100)) {
        Particle.publish("bright isn't a number?");
        bright=0;
    }
    //if (bright==100) bright=0; // 0 is max brightness, 1 is off, 255 is almost max.  see neopixel.ccp for docs
    minutes.setBrightness(bright);
    minutes.show();
    hours.setBrightness(bright);
    hours.show();
    Particle.publish(brightness);
    //exit(0);
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
    for(i=0; i<minutes.numPixels(); i++) {
      minutes.setPixelColor(i, Wheel((i+j) & 255));
    }
    minutes.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return minutes.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return minutes.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return minutes.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<minutes.numPixels(); i++) {
    minutes.setPixelColor(i, c);
    minutes.show();
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
