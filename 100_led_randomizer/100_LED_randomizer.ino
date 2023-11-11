#include <FastLED.h>

const uint8_t BTN_PIN = 9;
const uint8_t LED_PIN = 10;
const int num_pixels = 100;

const int step_size = 5;
const int brightness = 100;
const int del =  10; // Zero for no delay (millis)

CRGB leds[num_pixels];
uint8_t hues[num_pixels];

int num_effects = 5;
int effect_state = 3;
int last_btn_state;
int marquee_toggle = 0;

void led_fill()
{
  uint8_t new_color;
  
  for (int i = 0; i < num_pixels; i++)
  {
    new_color = random(255);
    leds[i].setHSV(new_color, 255, 255);
    hues[i] = new_color;
    FastLED.show();
  }
}

void effect_rand_1()
{
  int num;
  uint8_t new_color, old_color;
  int start_time = millis(); 
  
  num = random(num_pixels);
  new_color = (uint8_t) random(255);
  old_color = hues[num];

  if (new_color > old_color)
    for (int i = old_color; i < new_color; i += min(step_size, 255 - i))
    {
      // increment color
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  else
    for (int i = old_color; i > new_color; i -= min(step_size, i))
    {
      // decrement color
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  hues[num] = new_color;
}

void effect_rand_n(int n)
{
  // TODO: implement this
  int num;
  uint8_t new_color, old_color;
  int start_time = millis(); 
  
  num = random(num_pixels);
  new_color = (uint8_t) random(255);
  old_color = hues[num];

  if (new_color > old_color)
    for (int i = old_color; i < new_color; i += min(step_size, 255 - i))
    {
      // increment color
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  else
    for (int i = old_color; i > new_color; i -= min(step_size, i))
    {
      // decrement color
      debug(num, old_color, new_color, i);
      leds[num].setHSV(i, 255, 255);
      FastLED.show();
      delay(del);
    }
  hues[num] = new_color;
}

void effect_chase()
{
  uint8_t new_color = random(255);
  
  for (int i = num_pixels - 2; i >= 0; i--)
  {
    leds[i + 1] = leds[i];
  }
  
  leds[0].setHSV(new_color, 255, 255);
  FastLED.show();
  delay(del * 10);
}

void effect_marquee()
{
  int color_a = 0;
  int color_b = 128;

  for (int i = 0; i < num_pixels; i++)
  {
    if (marquee_toggle)
    {
      if (i % 2 == 0)
        leds[i].setHue(color_a);
      else
        leds[i].setHue(color_b);
      
      marquee_toggle = 0;
    }
    else
    {
      if (i % 2 == 0)
        leds[i].setHue(color_b);
      else
        leds[i].setHue(color_a);
      
      marquee_toggle = 1;
    }
  }

  FastLED.show();
  delay(del);
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting");
  pinMode(BTN_PIN, INPUT_PULLUP);
  last_btn_state = digitalRead(BTN_PIN);
  
  FastLED.addLeds<WS2811, LED_PIN>(leds, num_pixels);
  FastLED.setBrightness(brightness);
  
  led_fill();
}

void loop() 
{
  // detect button press
  int button_state = digitalRead(BTN_PIN);

  if (last_btn_state != button_state)
  {
    delay(50);

    if (button_state == LOW)
    {
      effect_state = (effect_state + 1) % num_effects;
    }
  }

  switch (effect_state)
  {
    case 0:
      effect_rand_1();
      break;
    case 1:
      effect_rand_n(10);
      break;
    case 2:
      effect_rand_n(num_pixels);
    case 3:
      effect_chase();
      break;
    case 4:
      effect_chase();
      break;
  }
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
