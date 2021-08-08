/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
#define BUTTON_PIN   0
boolean oldState = HIGH;

BleKeyboard bleKeyboard;



void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  bleKeyboard.begin();
}

void loop() {
  boolean newState = digitalRead(BUTTON_PIN);
  if(bleKeyboard.isConnected()) {
 //   while (newState);
    if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
        delay(20);
    // Check if button is still low after debounce.
        newState = digitalRead(BUTTON_PIN);
        if(newState == LOW) {      // Yes, still low
        Serial.println("Sending 'Password'...");
        bleKeyboard.print("Shiyishu2020!");
        delay(1000);
        bleKeyboard.releaseAll();
      // Set the last-read button state to the old state.
      }
     }
  }
  oldState = newState;
  delay(20);
}
