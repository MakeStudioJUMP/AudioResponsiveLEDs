#include <FastLED.h>                 	   // https://github.com/FastLED/FastLED 
#define NUM_LEDS 150              	  // Number of LEDs 
#define DATA_PIN 2                 	 // Connect your addressable LED strip to this pin. 
#define SENSITIVITY 300         	  // Ranges from 0 to 1023 
#define MAX_BRIGHTNESS 100          // Ranges from 0 to 255 
#define ENVELOPE_PIN A0                  // Connect sound detector to this pin 
#define SATURATION 255                    // Ranges from 0 to 255 
#define MINVAL 80

CRGB leds[NUM_LEDS];

int analogVal;
int count = 1;

void setup() {
 Serial.begin(9600); 
 pinMode(ENVELOPE_PIN, INPUT); 
 FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 

 // Turn off all LEDs initially
 for(int i=0; i < NUM_LEDS; i++) { 
    leds[i] = CRGB::Black; 
 }
 FastLED.setBrightness(80);
 FastLED.show();  
}

void loop() {
  analogVal = analogRead(ENVELOPE_PIN); 
  Serial.print("Sound Level: ");
  Serial.println(analogVal); 

  if (analogVal > SENSITIVITY) { 
    analogVal = SENSITIVITY; 
  } 
  if (analogVal < MINVAL) { 
    analogVal = 0; 
  } 

  colorPulse();
  FastLED.show();
  
  count++;
  if(count >= 101) count = 0;
}

void colorPulse() { 
  int hue = map(analogVal, MINVAL, SENSITIVITY, 0, 255);  // Changes color based on sound
  int factor = map(analogVal, MINVAL, SENSITIVITY, 5, 30); // More sound = more pulses
  int pulseSpeed = map(analogVal, MINVAL, SENSITIVITY, 50, 5); // Faster pulse when louder
  
  CRGB dim_val = CHSV(hue, 180, 100);  // Dim background color
  CRGB bright_val = CHSV(hue, 255, 255); // Bright center color

  int div = NUM_LEDS / factor;

  for(int i = 0; i <= div; i++) { 
    for(int j = 0; j <= factor - 1; j++) { 
      if(i > div * j) leds[i - (1 + div * j)] = dim_val;
      if(j > 0 && i < div * j) leds[i + div * j - 1] = dim_val;
    }
    
    leds[i] = bright_val; // Center of pulse
    
    for(int j = 0; j <= factor - 1; j++) { 
      if(i >= div * j) leds[i - div * j] = bright_val; // Behind the center
      if(j > 0 && i < div * j) leds[i + div * j] = bright_val; // In front of center
    }
    
    FastLED.show();
    delay(pulseSpeed); // Adjusted speed based on sound
  }
}
