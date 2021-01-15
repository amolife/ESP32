#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_APDS9960.h"

#define NUMPIXELS 8
#define LED_PIN 12
#define debug false
#define INT_PIN 2

//int r = 0, g = 0, b = 0;

//uint16_t r, g, b, c;
uint16_t r, g, b, c;

//byte gammatable[256];

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB+NEO_KHZ800);
Adafruit_APDS9960 apds;

void setup() {
//  if(debug){
//    Serial.begin(115200);
//  }

  Serial.begin(115200);
//  while (!Serial); // Wait for serial monitor to open
  pixels.begin();
  pixels.setBrightness(150);
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");
  //enable color sensign mode
  apds.enableColor(true);
  apds.enableProximity(true);
  apds.enableGesture(true);
//  for (int i = 0; i < 256; i++){
//    float x = i;
//    x /= 255;
//    x = pow(x, 2.5);
//    x *= 255;
//    gammatable[i] = x;
//  }
}

void loop(){

//    uint8_t gesture = apds.readGesture();
//    if(gesture == APDS9960_DOWN) Serial.println("v");
//    if(gesture == APDS9960_UP) Serial.println("^");
//    if(gesture == APDS9960_LEFT) Serial.println("<");
//    if(gesture == APDS9960_RIGHT) Serial.println(">");

  //wait for color data to be ready
//  while(!apds.colorDataReady()){
//    delay(5);
//  }
  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);
//  Serial.print("red: ");
//  Serial.print(r);
//  Serial.print(" green: ");
//  Serial.print(g);
//  Serial.print(" blue: ");
//  Serial.print(b);
//  Serial.print(" clear: ");
//  Serial.println(c);
//  Serial.println();

  delay(500);

  r=map(r,0,4096,0,255);
  g=map(g,0,4096,0,255);
  b=map(b,0,4096,0,255);  

  Serial.print("red: ");
  Serial.print(r);
  Serial.print(" green: ");
  Serial.print(g);
  Serial.print(" blue: ");
  Serial.print(b);
  Serial.print(" clear: ");
  Serial.println(c);
  Serial.println();
  delay(500);


  for(int i = 0; i < NUMPIXELS; i++){
//    pixels.setPixelColor(i, pixels.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]));
    pixels.setPixelColor(i, pixels.Color(r*(i+10), g*(i+10), b*(i+10)));
    //pixels.setPixelColor(i, pixels.Color(r, g, b));
    
  }
  pixels.show();
}
