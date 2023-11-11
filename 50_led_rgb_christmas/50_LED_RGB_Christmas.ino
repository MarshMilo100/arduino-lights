#include <FastLED.h>

const uint8_t LED_PIN = 2;
const int num_pixels = 50;

const int step_size = 10;
const int brightness = 100;
const int del = 20; // Zero for no delay

CRGB leds[num_pixels];
uint8_t hues[num_pixels];

void setup() 
{
  // put your setup code here, to run once:
  uint8_t new_color;
  
  Serial.begin(115200);
  Serial.println("Starting");
  
  FastLED.addLeds<WS2811, LED_PIN>(leds, num_pixels);
  FastLED.setBrightness(brightness);
  
  for (int i = 0; i < num_pixels; i++)
  {
    new_color = random(255);
    leds[i].setHSV(new_color, 255, 255);
    hues[i] = new_color;
    FastLED.show();
  }
}

void loop() 
{
  int num;
  uint8_t new_color, old_color;
  int start_time = millis(); 
  
  num = random(num_pixels);
  new_color = (uint8_t) random(255);
  old_color = hues[num];

  if (new_color > old_color)
    for (uint8_t i = old_color; i < new_color; i += min(step_size, 255 - i))
    {
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  else
    for (uint8_t i = old_color; i > new_color; i -= min(step_size, i))
    {
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  hues[num] = new_color;
}

void debug(int num, uint8_t old_color, uint8_t new_color, uint8_t current_color)
{
  Serial.print("Pixel ");
  Serial.print(num);
  Serial.print(": ");
  Serial.print(old_color);
  Serial.print(" -> ");
  Serial.print(new_color);
  Serial.print(" : ");
  Serial.println(current_color);
}
