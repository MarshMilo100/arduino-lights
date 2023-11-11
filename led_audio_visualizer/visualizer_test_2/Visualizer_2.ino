#include <arduinoFFT.h>
#include <Adafruit_NeoPixel.h>

// Constants
#define AUDIO_IN_PIN  A0  // Audio input
#define BTN_PIN       2   // Button intput 
#define LED_PIN       3   // Data pin to communicate with display

#define SAMPLES       64  // Must be a power of 2
#define SAMPLING_FREQ 40000
#define NOISE         500

#define XRES          32  // Total number of  columns in the display, must be <= SAMPLES/2
#define YRES          8   // Total number of  rows in the display

// Variables
double vReal[SAMPLES];
double vImag[SAMPLES];
char data_avgs[XRES];

int yvalue;
int displaycolumn , displayvalue;
int peaks[XRES];
const int buttonPin = 4;    // the number of the pushbutton pin
int state = HIGH;             // the current reading from the input pin
int previousState = LOW;   // the previous reading from the input pin
int displaymode = 1;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
const uint16_t num_pixels = 256;

unsigned long newTime;
unsigned int sampling_period_us;
int MY_ARRAY[]={0, 128, 192, 224, 240, 248, 252, 254, 255}; // default = standard pattern

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_pixels, LED_PIN, NEO_GRB + NEO_KHZ800);
arduinoFFT FFT = arduinoFFT();

void setup() 
{
  // Input Setup
  ADCSRA = 0b11100101;      // set ADC to free running mode and set pre-scalar to 32 (0xe5)
  ADMUX = 0b00000000;       // use pin A0 and external voltage reference
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));

  // Output Setup
  strip.begin();
  strip.setBrightness(64);
  strip.show();
  Serial.begin(115200);
  delay(50);
}

void loop() 
{
  // Sampling
  for(int i = 0; i < SAMPLES; i++)
  {
    newTime = micros();
    
    while (!(ADCSRA & 0x10));   // wait on ADIF bit
      ADCSRA = 0b11110101;      // clear ADIF bit
      
    vReal[i] = (double)ADC;
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }                      
  }

  // FFT
  FFT.DCRemoval();
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
  // Transform Output To Match Display
  int step = (SAMPLES/2)/XRES; 
  int c=0;
  
  for(int i = 0; i < (SAMPLES / 2); i += step)  
  {
    data_avgs[c] = 0;
    for (int k = 0; k < step; k++) 
    {
        data_avgs[c] = data_avgs[c] + vReal[i + k];
    }
    data_avgs[c] = data_avgs[c] / step; 
    c++;
  }

  strip.clear();
    
  // Display Output
  for(int i = 0; i < XRES; i++)
  {
    data_avgs[i] = constrain(data_avgs[i], 0, 80);            // set max & min values for buckets
    data_avgs[i] = map(data_avgs[i], 0, 80, 0, YRES);         // remap averaged values to YRES
    yvalue = data_avgs[i];

    // Apply Effects / Animations
    peaks[i] = peaks[i] - 1;                                  // decay by one light
    
    if (yvalue > peaks[i]) 
      peaks[i] = yvalue ;
    
    yvalue = peaks[i];    
    //displayvalue = MY_ARRAY[yvalue];
    setColumn(i, yvalue);
  }

  // Cleanup
  strip.show();
  displayModeChange ();                                       // check if button pressed to change display mode
  delay(10);
} 

void setColumn(int col, int value)
{
  if (col % 2 != 0)
  {
    // Odd columns
    for (int i = 0; i < YRES; i++)
      if (i < value)
        strip.setPixelColor(col * YRES + i, strip.Color(128, 0, 128));
//      else
//        strip.setPixelColor(col * YRES + i, strip.Color(0, 0, 0));
  }
  else
  {
    // Even columns
    for (int i = 0; i < YRES; i++)
      if (i < value)
        strip.setPixelColor((col + 1) * YRES - i, strip.Color(128, 0, 128));
//      else
//        strip.setPixelColor((col + 1) * YRES - i, strip.Color(0, 0, 0));
  }
  
  Serial.print("Col: ");
  Serial.print(col);
  Serial.print(", Val: ");
  Serial.println(value);
}

void displayModeChange() 
{
  int reading = digitalRead(buttonPin);
  
  if (reading == HIGH && previousState == LOW && millis() - lastDebounceTime > debounceDelay) // works only when pressed
  {
    lastDebounceTime = millis();
    //TODO: Implement Mode Switching
  }
  previousState = reading;
}
