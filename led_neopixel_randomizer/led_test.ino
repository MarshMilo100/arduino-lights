#include <Adafruit_NeoPixel.h>

const uint8_t LED_PIN = 2;
const uint16_t num_pixels = 100;

const int brightness = 100;
const int variability = 30;
const int del = 1000; // Zero for no delay
const bool individual_delay = false;

// orange, purple, green, blue
int red_s[] =   {255, 255, 75,  45,  75};
int green_s[] = {85,  85,  15,  200, 125};
int blue_s[] =  {15,  15,  250, 15,  250};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_pixels, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() 
{
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(brightness);
  strip.clear();
  Serial.begin(115200);
}

void loop() 
{
  Serial.println("Starting");
  int red, green, blue;
  int num;
  
  for (int i = 0; i < num_pixels; i++)
  {
//    num = random(4);
//    red = min(max(red_s[num], 0), 255);
//    green = min(max(green_s[num], 0), 255);
//    blue = min(max(blue_s[num], 0), 255);
    red = random(255);
    green = random(255);
    blue = random(255);

    strip.setPixelColor(i, strip.Color(red, green, blue));

    if (individual_delay)
    {
      if (delay > 0)
        delay(del);
      strip.show();
    }
  }

  if (!(individual_delay))
  {
    if (delay > 0)
        delay(del);
    strip.show();
  }
}
