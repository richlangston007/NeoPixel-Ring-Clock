// This #include statement was automatically added by the Particle IDE.

/*
 * Based on example particle neopixel application, which was ported from Adafruit
 * rich@thelangstons.org
 *
 */

#include "application.h"
#include "neopixel.h"
#include "adafruit-led-backpack.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
Adafruit_NeoPixel minutes = Adafruit_NeoPixel(60, D2, WS2812B);
Adafruit_NeoPixel hours = Adafruit_NeoPixel(24, D3, WS2812B);
Adafruit_24bargraph tempBar = Adafruit_24bargraph();
Adafruit_7segment tempNum = Adafruit_7segment();

float tempF=20;
float oldTemp=0;
int modeButton = D4;

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
uint32_t rainbowColors[7] = {0xFF0000,0xFF7F00,0xFFFF00,0x00FF00, 0x0000FF,0x4B0082,0x8B00FF};
int looper=0;
int firstTime=1;
int mode=2;
//int firstSecond=1;

void setup()
{
  tempNum.begin(0x71);
  tempNum.setBrightness(150);
  tempNum.writeDisplay();
  minutes.begin();
  minutes.show(); // Initialize all pixels to 'off'
  hours.begin();
  hours.show(); // Initialize all pixels to 'off'
  pinMode(modeButton, INPUT);
  Serial.begin(9600);
  Time.zone(-8); //PST
  Serial.println("start serial debug channel");
  Spark.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);
  tempBar.begin(0x70);  // pass in the address
  for (int c=0;c<24;c++)
    tempBar.setBar(c,LED_OFF);

}
void loop()
{
  uint32_t markers=0;
  timeHours = Time.hourFormat12();
  timeMinutes=Time.minute();
  timeSeconds=Time.second();
  if (timeMinutes % 5 == 0) Spark.publish("get_weather");
  int light=analogRead(photoresistor);
  Serial.print("photo:");
  Serial.println(light);
  // Pressing the button will increment through all the clock modes
  if (digitalRead(modeButton)) {
    mode++;
    delay(200);
    if (mode==3) mode=1;
  }
  //
  //  Adjust the brightness so no one gets hurt
  if (light<2800) {
      hours.setBrightness(100);
      hours.show();
      minutes.setBrightness(120);
      minutes.show();
    } else if (light<1000) {
      hours.setBrightness(40);
      hours.show();
      minutes.setBrightness(50);
      minutes.show();
      } else {
      hours.setBrightness(200);
      hours.show();
      minutes.setBrightness(200);
      minutes.show();
    }

if (mode==1) {
// Deal with the seconds and minutes on big ring
  if ((timeSeconds==0) && (firstTime)) {
    looper++;
    if (looper==8) looper=1; //cycle the rainbow
    firstTime=0;
  }
  if (timeSeconds==1) firstTime=1;
  if (timeSeconds==0) {
    minutes.setPixelColor(59,rainbowColors[looper]);
    minutes.setPixelColor(58,rainbowColors[looper]);
  }
  minutes.setPixelColor(timeMinutes-2,rainbowColors[looper]);
  minutes.setPixelColor(timeMinutes-1,0x333333);
  minutes.setPixelColor(timeMinutes,0x888888);
  minutes.setPixelColor(timeMinutes+1,0x333333);
  minutes.setPixelColor(timeSeconds,rainbowColors[looper]);
  minutes.show();

//Deal with the hours on the small ring
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
    }
//    Serial.print("time:");
//    Serial.println(hourLED);

  hours.show();
} else if (mode=2) {
  if (timeSeconds==0) minutes.setPixelColor(59,0);
  if (timeSeconds!=0) minutes.setPixelColor(timeSeconds-1,0x00);
  minutes.setPixelColor(timeMinutes-2,0x00);
  minutes.setPixelColor(timeMinutes-1,0x333333);
  minutes.setPixelColor(timeMinutes,0x888888);
  minutes.setPixelColor(timeMinutes+1,0x333333);
  minutes.setPixelColor(timeSeconds,0x008800);
  //markers
  if (Time.hour()>5) markers=0xFFD700;
  if (Time.hour()>14) markers=0x1E90FF;
  if (Time.hour()>20) markers=0x4B0082;
  if (Time.hour()>22) markers=0x182A71;
  minutes.setPixelColor(14,markers);
  minutes.setPixelColor(44,markers);
  minutes.setPixelColor(0,markers);
  minutes.setPixelColor(29,markers);
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
    }
//    hours.setPixelColor(0,0x4B0082);
//    hours.setPixelColor(23,0x4B0082);
//    Serial.print("time:");
//    Serial.println(hourLED);

  hours.show();
}

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
// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    // Example from particle documentation https://docs.particle.io/guide/tools-and-features/webhooks/
    // Important note!  -- Right now the response comes in 512 byte chunks.
    //  This code assumes we're getting the response in large chunks, and this
    //  assumption breaks down if a line happens to be split across response chunks.
    //
    // Sample data:
    //  <location>Minneapolis, Minneapolis-St. Paul International Airport, MN</location>
    //  <weather>Overcast</weather>
    //  <temperature_string>26.0 F (-3.3 C)</temperature_string>
    //  <temp_f>26.0</temp_f>


    String str = String(data);
    String tempStr = tryExtractString(str, "<temp_f>", "</temp_f>");

    if (tempStr != NULL) {
        Serial.println("The temp is: ");
        Serial.println(tempStr);

        tempF = tempStr.toFloat();
        if (tempF==oldTemp) return;
        oldTemp=tempF;
        Serial.println("float temp in function: ");
        Serial.println(tempF);
        int bars=.24*tempF;
        for (int c=0;c<24;c++)
        	tempBar.setBar(c,LED_OFF);
        for (int c=0;c<bars;c++) {
          tempBar.setBar(c,LED_GREEN);
          if (bars<9) tempBar.setBar(c,LED_YELLOW);
          if (bars<5) tempBar.setBar(c,LED_RED);
          if (bars>22) tempBar.setBar(c,LED_RED);
        }
        tempBar.writeDisplay();
        tempNum.print(tempF);
        tempNum.writeDisplay();
      }
}

// Returns any text found between a start and end string inside 'str'
// example: startfooend  -> returns foo
String tryExtractString(String str, const char* start, const char* end) {
    if (str == NULL) {
        return NULL;
    }

    int idx = str.indexOf(start);
    if (idx < 0) {
        return NULL;
    }

    int endIdx = str.indexOf(end);
    if (endIdx < 0) {
        return NULL;
    }

    return str.substring(idx + strlen(start), endIdx);
}
