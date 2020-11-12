/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

int led1 = 25;
int led2 = 26;
int led3 = 27;

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <HTTPClient.h>

//#include "time.h"
//const char* ntpServer = "ntp.aliyun.com";
//const long  gmtOffset_sec = 3600;
//const int   daylightOffset_sec = 3600;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CcEGm3vk876OHzFWlABK9knBB0WH1eOa";
// Set password to "" for open networks.
char ssid[] = "CMCC-Stone";
char pass[] = "shiyongqian";
///////String textToMe;
///////const char Mytext[]* = "Say what?";

WidgetTerminal terminal(V8);

#define DHTPIN 4          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
BlynkTimer timer1;

String payload;

long unix_time;
//char WebhookToWeather[] = "";

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);

  u8g2.setFont(u8g2_font_unifont_t_chinese1);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  
  //--------------utc----------------------//  
  //u8g2.setCursor(0, 13);
  //u8g2.print(unix_time);
  //--------------utc----------------------//  

  u8g2.setCursor(0, 13);
  u8g2.print(payload.substring(173,193));

  
  u8g2.setCursor(0, 29);
  u8g2.print("T:");
  u8g2.setCursor(20, 29);
  u8g2.print(t);

//  u8g2.drawHLine(0, 15, 128);
//  u8g2.drawHLine(0, 31, 128);
//  u8g2.drawHLine(0, 47, 128);
  
  u8g2.setCursor(65, 29);
  u8g2.print("H:");
  u8g2.setCursor(85, 29);
  u8g2.print(h);
  
  u8g2.setCursor(0, 45);
//  u8g2.print("IP:");
//  u8g2.setCursor(30, 45);
  u8g2.print(WiFi.localIP());

//  u8g2.setCursor(0, 55);//设置文字显示位置
//  u8g2.print(textToMe);//将接收到的文字显示出来
  
  u8g2.sendBuffer();

//  Blynk.virtualWrite(V7, "http://api.seniverse.com/v3/weather/now.json?key=SwPpfCT45fY_nBknj&location=beijing&language=zh-Hans&unit=c");

}


void http_get() {
    // wait for WiFi connection
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("http://api.seniverse.com/v3/weather/now.json?key=SwPpfCT45fY_nBknj&location=beijing&language=zh-Hans&unit=c"); //HTTP
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
}


////-------------UTC-------------//
//void requestTime() {
//  Blynk.sendInternal("rtc", "sync");
//}
//
//BLYNK_WRITE(InternalPinRTC) {
//  unix_time = param.asLong();
//  Serial.print("Unix time: ");
//  Serial.print(unix_time);
//  Serial.println(); 
//}
////-------------UTC-------------//

void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(5000L, sendSensor);

  //-----------------------------
  //设置 channel 0 频率为 312500 Hz
  sigmaDeltaSetup(0, 312500);
  //IO口 led1 附加到 channel 0
  sigmaDeltaAttachPin(led1,0);

  sigmaDeltaSetup(1, 312500);
  sigmaDeltaAttachPin(led2,1);

  sigmaDeltaSetup(2, 312500);
  sigmaDeltaAttachPin(led3,2);
 
  //channel 0，1，2 初始状态为关闭
  sigmaDeltaWrite(0, 0);
  sigmaDeltaWrite(1, 0);
  sigmaDeltaWrite(2, 0);
//-----------------------------

  u8g2.begin();
  u8g2.enableUTF8Print();  
  Blynk.begin(auth, ssid, pass);

//------------------------//
  terminal.clear();
  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Fuck' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();
//------------------------//

//-----------UTC-------------//
//  timer.setInterval(10000L, requestTime);
//------------UTC------------//


  timer1.setInterval(10000L, http_get);

}


//BLYNK_WRITE(V7)
//{ 
//  //WebhookToWeather[] = param.asStr();
//  Serial.println("WebHook data:");
//  Serial.println(param.asStr());
//}




BLYNK_WRITE(V8)
{
   
  terminal.println(String(dht.readTemperature()));
  delay(3000);
  terminal.println(String(dht.readHumidity()));
  delay(3000);

  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Fuck") == param.asStr()) {
    terminal.println("You said: 'Fuck'") ;
    terminal.println("I said: 'you'") ;
  } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }
  String textToMe = param.asStr();
///////////  const char Mytext[]* = param.asStr();
  u8g2.setFont(u8g2_font_unifont_t_chinese1);  // 设置字体
  //字体列表https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.setFontDirection(0);//设置文字方向
//  u8g2.clearBuffer();
  u8g2.setCursor(0, 61);//设置文字显示位置
  u8g2.print(textToMe);//将接收到的文字显示出来
  u8g2.sendBuffer();
  delay(3000);
    // Ensure everything is sent
  terminal.flush();
}



BLYNK_WRITE(V10)
{
  int led1Value = param.asInt(); // assigning incoming value from pin V1 to a variable
  sigmaDeltaWrite(0, led1Value);
  // process received value
}

BLYNK_WRITE(V11)
{
  int led2Value = param.asInt(); // assigning incoming value from pin V1 to a variable
  sigmaDeltaWrite(1, led2Value);
  // process received value
}

BLYNK_WRITE(V12)
{
  int led3Value = param.asInt(); // assigning incoming value from pin V1 to a variable
  sigmaDeltaWrite(2, led3Value);
  // process received value
}

void loop()
{
  Blynk.run();
  timer.run();
  timer1.run();  
}
