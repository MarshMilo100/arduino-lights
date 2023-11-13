#include <FastLED.h>
#include <pixeltypes.h>

#undef SER_DEBUG

const uint8_t BTN_PIN = 9;  // Button input
const uint8_t LED_PIN = 10; // LED data output
const int num_pixels = 100; // Pixel count

const int step_size = 5;    // Gradual effect step size
const int brightness = 128; // Global brightness (0 - 255)
const int del = 10;         // Zero for no delay (millis)

CHSV hues[num_pixels];      // HSV color object (for storing hues)
CRGB leds[num_pixels];      // LED color objects

int num_effects = 5;        // Total number of effects defined
int effect_state = 4;       // Store active effect
int last_btn_state;         // Store button state
bool marquee_toggle = false;// Store Marquee state

void debug(int num, uint8_t old_color, uint8_t new_color)
{
    Serial.print("Pixel ");
    Serial.print(num);
    Serial.print(" : ");
    Serial.print(old_color);
    Serial.print(" -> ");
    Serial.print(new_color);
}

void set_color(int i, uint8_t new_hue)
{
    hues[i].setHSV(new_hue, 255, brightness);
    hsv2rgb_rainbow(hues[i], leds[i]);

#ifdef SER_DEBUG
    debug(i, hues[i].hue, new_hue, )
#endif /* SER_DEBUG */
}

void led_fill()
{
    uint8_t new_color;

    for (int i = 0; i < num_pixels; i++)
    {
        new_color = random(255);
        set_color(i, new_color);
        FastLED.show();
    }
}

void effect_rand_1()
{
    int num;
    uint8_t new_color, old_color;
    int start_time = millis();

    num = random(num_pixels);
    new_color = (uint8_t)random(255);
    old_color = hues[num][0];

    for (uint8_t color = old_color; color < new_color; color += min(step_size, 255 - color))
    {
        // increment color
        set_color(num, color);
        FastLED.show();
        delay(del);
    }
}

void effect_rand_n(int n)
{
    int num;
    uint8_t new_color, old_color;
    int start_time = millis();

    num = random(num_pixels);
    new_color = (uint8_t)random(255);
    old_color = hues[num][0];

    for (uint8_t color = old_color; color < new_color; color += min(step_size, 255 - color))
    {
        // increment color
        set_color(num, color);
        FastLED.show();
        delay(del);
    }
}

void effect_chase(int block_size)
{
    uint8_t new_color = random(255);

    for (int j = 0; j < block_size; j++)
    {
        for (int i = num_pixels; i >= 0; i--)
        {
            leds[i + 1] = leds[i];
        }

        set_color(0, new_color);
        FastLED.show();
        delay(del * 10);
    }
}

void effect_marquee()
{
    int color_a = 96;
    int color_b = 0;

    for (int i = 0; i < num_pixels; i += 2)
    {
        set_color(i,     marquee_toggle ? color_a : color_b);
        set_color(i + 1, marquee_toggle ? color_b : color_a);
    }

    marquee_toggle = !(marquee_toggle);
    FastLED.show();
    delay(del * 100);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Starting");
    Serial.print("Mode : ");
    Serial.println(effect_state);
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
        delay(100);

        if (button_state == LOW)
        {
            effect_state = (effect_state + 1) % num_effects;
            Serial.print("Mode : ");
            Serial.println(effect_state);
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
        break;
    case 3:
        effect_chase(num_pixels / 10);
        break;
    case 4:
        effect_marquee();
        break;
    }
}
