#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 2
#define WINDOW 20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void soundToColor(uint8_t wait, float sound){
  /*
   * takes a number 0-255 and loops through all the pixels to set them to that color
   */
  //Serial.print("color changed: ");
  //Serial.println(sound);
  for(int i=0; i<strip.numPixels(); i++) { //loop through all the pixels
      strip.setPixelColor(i, Wheel(sound));
      delay(wait);
      strip.show();
    }
}



float listen(){
   /*
   * Code below reads the voltage from the microphone and converts it to a color (0 - 255)
   */
  int count = 0;
  int max_val = -1;
  int mvg_avg = 0;
  int min_val = 256;
  float spread = 1.3;
  while(count < WINDOW) {
    count = count + 1;
        
    // read the input on analog pin 5:
    int sensorValue = analogRead(A5);
  
    // Convert the analog reading (which goes from 0 - 1023) to a color (0 - 255):
    float sound = sensorValue * (255.0 / 1023.0);
    // print out the value you read:
    //Serial.print("sounds: ");
    Serial.println(sound);
    
    
    
    if(sound > max_val) {
      max_val = sound;
    }
    if(sound < min_val) {
      min_val = sound;
    }
    mvg_avg = mvg_avg + (sound - mvg_avg)/count;
    
  }
  //Serial.println((max_val-mvg_avg)/2);
  //Serial.print("mvg avg: ");
  //Serial.println(mvg_avg);
  //Serial.print("max value: ");
  //Serial.println(max_val);
  return (max_val + mvg_avg)/2;

}


uint32_t Wheel(byte WheelPos) {
/*
 * Wheel transforms a number 0 to 255 to a color
 * Input a value 0 to 255 to get a color value.
 * The colours are a transition r - g - b - back to r.
 */
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


void loop() {
 
  soundToColor(75, listen());
  
}
