// Author: Leonardo La Rocca
// email: info@melopero.com
// 
// In this example it is shown how to configure the device to print out
// The rgbc (red, green, blue, clear) color values.
// 
// First make sure that your connections are setup correctly:
// I2C pinout:
// APDS9960 <------> Arduino MKR
//     VIN <------> VCC
//     SCL <------> SCL (12)
//     SDA <------> SDA (11)
//     GND <------> GND
// 
// Note: Do not connect the device to the 5V pin!

#include "Melopero_APDS9960.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 8
#define LED_PIN 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB+NEO_KHZ800);
Melopero_APDS9960 device;

void setup() {
  Serial.begin(9600); // Initialize serial comunication
  while (!Serial); // wait for serial to be ready

  pixels.begin();
  pixels.setBrightness(150);

  device.init(); // Initialize the comunication library
  device.reset(); // Reset all interrupt settings and power off the device

  device.enableAlsEngine(); // enable the color/ALS engine
  device.setAlsIntegrationTime(450); // set the color engine integration time
  device.updateSaturation(); // updates the saturation value, stored in device.alsSaturation

  // The device.alsSaturation variable represents the maximum value for the rgbc variables.
  // If you want a normalized value of the rgbc variables you have to divide them by the 
  // alsSaturation. 
  // device.red/green/blue/clear <-> Raw color value for red/green/blue/clear
  // device.alsSaturation <-> saturation value for rgbc (maximum value they can assume)
  // float norm_red = (float) device.red / (float) device.alsSaturation <-> normalized red value in range [0 - 1]

  device.wakeUp(); // wake up the device

}

void loop() {
  delay(450); 

  device.updateColorData(); // update the values stored in device.red/green/blue/clear

  Serial.println("Raw color data:");
  printColor(device.red, device.green, device.blue, device.clear); // print raw values
  Serial.println("Saturation value: ");
  Serial.println(device.alsSaturation); // print saturation

//  float r = (((float) device.red) / (float) device.alsSaturation); // normalized red (in range [0,1])
//  float g = (((float) device.green) / (float) device.alsSaturation); // normalized green (in range [0,1])
//  float b = (((float) device.blue) / (float) device.alsSaturation); // normalized blue (in range [0,1])
//  float c = (((float) device.clear) / (float) device.alsSaturation); // normalized clear (in range [0,1])

  int r=map(device.red,0,4096,0,255);
  int g=map(device.green,0,4096,0,255);
  int b=map(device.blue,0,4096,0,255);  
  int c=map(device.blue,0,4096,0,255);  

  Serial.println("Color data:");
  printColor(r,g,b,c); // print normalized values

    for(int i = 0; i < NUMPIXELS; i++){
//    pixels.setPixelColor(i, pixels.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]));
    pixels.setPixelColor(i, pixels.Color(r*(i+1), g*(i+1), b*(i+1)));
    //pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
      pixels.show();
    
}

//void printColor(float r, float g, float b, float c){
//  Serial.print("R: ");
//  Serial.print(r);
//  Serial.print(" G: ");
//  Serial.print(g);
//  Serial.print(" B: ");
//  Serial.print(b);
//  Serial.print(" C: ");
//  Serial.println(c);
//}

void printColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c){
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" C: ");
  Serial.println(c);
}
