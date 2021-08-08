/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
#define BUTTON_PIN   0

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  boolean newState = !digitalRead(BUTTON_PIN);
  if(bleKeyboard.isConnected()) {
    //Serial.println("Connected!");
    if (newState){
      //delay(100);
      Serial.println("Sending 'Password'...");
      bleKeyboard.print("Shiyishu2020!");
      bleKeyboard.releaseAll();
      delay(500);
      //while (newState);
      }
     }
  }
