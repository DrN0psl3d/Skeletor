#include <Adafruit_NeoPixel.h>

/*
 * #define AUTHOR   @Dr_n0psl3d
 * #define DATE     20180727
 * #define VERSION  1.0
 * #define URL      https://github.com/DrN0psl3d/Skeletor
 * #define DESC.    Code for Skeletor Badge
 */

#define DATA 3
#define GREEN 1
#define RED 4
#define NUMPIX 8

#define MORSE_ONTIME_SHORT 100
#define MORSE_ONTIME_LONG 3000
#define MORSE_DELAY_SPACE 1500
#define MORSE_DELAY_WORD 2500

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIX, DATA, NEO_GRB + NEO_KHZ800);

int defcon_morse_pattern[] = {1,0,0, -1, 0, -1, 0,0,1,0, -1, 1,0,1,0, -1, 1,1,1, -1, 1,0, -2, 0,0, -1, 
                              0,0,0, -2, 0,1, -1, 0,1,1, -1, 0, -1, 0,0,0, -1, 1,1,1, -1, 
                              1,1, -1, 0, -2};
int rtg_morse_pattern[] = {0,1,0, -1, 0, -1, 1,0,0, -2, 1, -1, 0, -1, 0,1, -1, 1,1, -2, 1,1,0, -1, 0,1,0, -1,
                           0,0, -1, 0,0,1,0, -1, 0,0,1,0, -1, 0,0, -1, 1,0, -2};
int dr_morse_pattern[] = {1,0,0, -1, 0,1,0, -1, 0,1,0,1,0,1, -1, 1,0, -1, 1,1,1,1,1, -1, 0,1,1,0, -1,
                          0,0,0, -1, 0,1,0,0, -1, 0,0,0,1,1, -1, 1,0,0, -2};

int last_pick = -1;

void setup() 
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pixels.begin();
  pixels.setBrightness(32);
  pixels.show();
}

void loop()
{
  int pick = random(0,13);
  while(pick == last_pick)
  {
    pick = random(0,13);
  }
  last_pick = pick;
  switch(pick)
  {
    case 0:
      morse(defcon_morse_pattern);
    case 1:
      morse(rtg_morse_pattern);
    case 2:
      morse(dr_morse_pattern);
    case 3:
      blinkLeds(random(0,2), random(0,4) * 100);
    case 4:
      blinkLeds(random(0,2), random(0,4) * 100);
    case 5:
      blinkLeds(random(0,2), random(0,4) * 100);
    case 6:
      for(int i = 0; i < random(1,5); i++)
      {
        rainbow(10); 
      }
    case 7:
      for(int i = 0; i < random(1,5); i++)
      {
        rainbow(10); 
      }
    case 8:
      blinkFaster(random(0,2), 1000, 10);
    case 9:
      blinkFaster(random(0,2), 1000, 10);
    case 10:
      blinkFaster(random(0,2), 1000, 10);
    case 11:
      chaser(random(2,5), random(0,2));
    case 12:
      chaser(random(2,5), random(0,2));
  }
}

void clearLeds()
{
  for(int i = 0; i < NUMPIX+1;i++)
  {
    pixels.setPixelColor(i, 0);
  }
  pixels.show();
}

void blinkFaster(uint8_t color, uint8_t start_delay, uint8_t stop_delay)
{
  if(stop_delay >= start_delay)
  {
    return;
  }
  for(int i=start_delay; i > stop_delay; i -= 5)
  {
    blinkLeds(color, 100 );
    delay(i);
  }
  blinkLeds(!color, 1000);
}

void blinkLeds(uint8_t color, uint8_t wait)
{
  if(color) //green
  {
    digitalWrite(RED, HIGH);
    delay(wait);
    digitalWrite(RED, LOW);
  }
  else //red
  {
    digitalWrite(GREEN, HIGH);
    delay(wait);
    digitalWrite(GREEN, LOW);
  }
}

uint32_t getRandomColor()
{
  randomSeed(analogRead(A0));
  uint8_t r = random(0, 5) * 64;
  uint8_t g = random(0, 5) * 64;
  uint8_t b = random(0, 5) * 64;
  return pixels.Color(r,g,b);
}

void chaser(uint8_t loops, uint8_t direction)
{
  uint8_t index = direction * NUMPIX; //0 for going left, 1*NUMPIX for going right
  for(int l = loops; l > 0; l--)
  {
   if(index > 0) // go right
  {
    for(int i = index; i >= 0; i--)
    {
      if(i == index)
      {
        pixels.setPixelColor(0, 0);
      }
      pixels.setPixelColor(i, getRandomColor());
      pixels.show();
      delay(100);
      pixels.setPixelColor(i+1, 0);
    }
  }
  else //go left
  {
    for(int i = 0; i < NUMPIX; i++)
    {
      if(i == NUMPIX)
      {
        pixels.setPixelColor(NUMPIX, 0);
      }
      pixels.setPixelColor(i, getRandomColor());
      pixels.show();
      delay(100);
      pixels.setPixelColor(i-1, 0);
    }
  } 
  }
  clearLeds();
}

void morse(int pattern[])
{
    for(int i=0; i<(sizeof(defcon_morse_pattern)/sizeof(int)); i++)
    {
      if(defcon_morse_pattern[i] == 0) //short
      {
        blinkLeds(0, MORSE_ONTIME_LONG);
        delay(250);
      }
      if(defcon_morse_pattern[i] == 1) //long
      {
        blinkLeds(1, MORSE_ONTIME_LONG);
        delay(250);
      }
      if(defcon_morse_pattern[i] == -1) //space
      {
        delay(MORSE_DELAY_SPACE);
      }
      if(defcon_morse_pattern[i] == -2) //word
      {
        delay(MORSE_DELAY_WORD);
      }
   }
}

// Thanks to: https://codebender.cc/sketch:80438#Neopixel%20Rainbow.ino lol

void rainbow(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) 
    {
      pixels.setPixelColor(i, Wheel((i*1+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
  clearLeds();
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
