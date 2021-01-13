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
//#define BLYNK_PRINT Serial


#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <HTTPClient.h>

#include "time.h"
const char* ntpServer = "ntp.aliyun.com";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CcEGm3vk876OHzFWlABK9knBB0WH1eOa";
// Set password to "" for open networks.
char ssid[] = "CMCC-Stone";
char pass[] = "shiyongqian";


int led1 = 25;  //led GPIO25
int led2 = 26;
int led3 = 27;

String payload;
long unix_time;
String Xinzhi_wenzi;
String Xinzhi_wendu;
struct tm timeinfo;
String textToMe;

#define DHTPIN 4          // DHT GPIO
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

WidgetTerminal terminal(V8);
BlynkTimer timer;
//BlynkTimer timer1;


// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{ 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
Serial.println(h);
Serial.println(t);


  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    //return;    //应该解决了DHT11煞笔以后，系统异常问题
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);

//  u8g2.setFont(u8g2_font_unifont_t_chinese3);
//  u8g2.setFont(u8g2_font_wqy12_t_gb2312);

//  u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();

//    u8g2.setContrast(20);
  //--------------utc----------------------//  
  //u8g2.setCursor(0, 13);
  //u8g2.print(unix_time);
  //--------------utc----------------------//  

  
  u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
  
 // u8g2.setCursor(0, 25);
//  u8g2.print(payload.substring(173,193));
//  u8g2.print("北京:");
  u8g2.setCursor(0, 32);
  u8g2.print(Xinzhi_wenzi);
 // u8g2.setCursor(65, 25);
//  u8g2.print("室外:");
  u8g2.setCursor(102, 32);
  u8g2.print("°C");

//  u8g2.setCursor(0, 55);
//  u8g2.print("温:");
  u8g2.setCursor(46, 62);
  u8g2.print("°C");
//  u8g2.setCursor(69, 55);
//  u8g2.print("湿:");
  u8g2.setCursor(120, 62);
  u8g2.print("%");

//  u8g2.setCursor(0, 61);//设置文字显示位置
//  u8g2.print(&timeinfo, "%Y.%m.%d %a %H:%M");//Ali NTP date and time
//-----------------------------data------------------------
  u8g2.setFont(u8g2_font_courB14_tf);
  u8g2.setCursor(64, 32);
//  u8g2.print("-20");
  u8g2.print(Xinzhi_wendu);
  u8g2.setCursor(0,62);
  u8g2.print(t,1);   // 0.00 to 0.0
  u8g2.setCursor(74, 62);
  u8g2.print(h,1);   // 0.00 to 0.0
//-----------------------------data------------------------

  u8g2.sendBuffer();
  
//  u8g2.setCursor(0, 45);
//  u8g2.print("IP地址:");
//  u8g2.setCursor(40, 45);
//  u8g2.print(WiFi.localIP());
//
////  u8g2.setCursor(0, 55);//设置文字显示位置
////  u8g2.print(textToMe);//将接收到的文字显示出来
//  u8g2.setCursor(0, 61);//设置文字显示位置
//  u8g2.print(&timeinfo, "%Y.%m.%d %a %H:%M");//Ali NTP date and time
//  u8g2.sendBuffer();
  
  terminal.println("心知天气预报：");
  terminal.print("北京：");
  terminal.print(Xinzhi_wenzi);
  terminal.print("  室外：");
  terminal.print(Xinzhi_wendu);
  terminal.print("°C");
  terminal.println(" ");
//  Blynk.virtualWrite(V7, "http://api.seniverse.com/v3/weather/now.json?key=SwPpfCT45fY_nBknj&location=beijing&language=zh-Hans&unit=c");
  terminal.flush();
}

void printLocalTime()
{
//  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%Y-%b-%d,%a %H:%M:%S");
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
    
    arduino_json();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
}

void arduino_json(){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 210;
  DynamicJsonDocument doc(capacity);
  
//  const char* json = "{\"results\":[{\"location\":{\"id\":\"WX4FBXXFKE4F\",\"name\":\"北京\",\"country\":\"CN\",\"path\":\"北京,北京,中国\",\"timezone\":\"Asia/Shanghai\",\"timezone_offset\":\"+08:00\"},\"now\":{\"text\":\"多云\",\"code\":\"4\",\"temperature\":\"9\"},\"last_update\":\"2020-11-05T22:00:00+08:00\"}]}";
  
  deserializeJson(doc, payload);
  
  JsonObject results_0 = doc["results"][0];
  
//  JsonObject results_0_location = results_0["location"];
//  const char* results_0_location_id = results_0_location["id"]; // "WX4FBXXFKE4F"
//  const char* results_0_location_name = results_0_location["name"]; // "北京"
//  const char* results_0_location_country = results_0_location["country"]; // "CN"
//  const char* results_0_location_path = results_0_location["path"]; // "北京,北京,中国"
//  const char* results_0_location_timezone = results_0_location["timezone"]; // "Asia/Shanghai"
//  const char* results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"
  
  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"]; // "多云"
//  const char* results_0_now_code = results_0_now["code"]; // "4"
  const char* results_0_now_temperature = results_0_now["temperature"]; // "9"
  
//  const char* results_0_last_update = results_0["last_update"]; // "2020-11-05T22:00:00+08:00"

  Serial.print("北京现在天气是："); 
  Serial.println(results_0_now_text); 
  Xinzhi_wenzi = results_0_now_text;
  Serial.print("温度是："); 
  Serial.println(results_0_now_temperature); 
  Xinzhi_wendu = results_0_now_temperature;
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
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);   
  Serial.begin(9600);
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(9000L, sendSensor);

  //-----------------------------
  //设置 channel 0 频率为 312500 Hz
  sigmaDeltaSetup(0, 312500);
  sigmaDeltaAttachPin(led1,0);  //IO口 led1 附加到 channel 0
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
  terminal.println(F("-----------------------"));
  terminal.println(WiFi.localIP());
  terminal.println(F("Type 'Fuck' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();
//------------------------//

//-----------UTC-------------//
//  timer.setInterval(10000L, requestTime);
//------------UTC------------//

  timer.setInterval(10000L, http_get);


  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

}

//BLYNK_WRITE(V7)
//{ 
//  //WebhookToWeather[] = param.asStr();
//  Serial.println("WebHook data:");
//  Serial.println(param.asStr());
//}


BLYNK_WRITE(V8)
{
//  terminal.println(String(dht.readTemperature()));
//  delay(3000);
//  terminal.println(String(dht.readHumidity()));
//  delay(3000);

  if (String("Fuck") == param.asStr()) {
    terminal.println("You said: 'Fuck'") ;
    terminal.println("I said: 'you'") ;
  } else {
    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }
  
  terminal.flush();
  textToMe = param.asStr();

//  u8g2.setFont(u8g2_font_unifont_t_chinese1);  // 设置字体
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  u8g2.setFontDirection(0);//设置文字方向
  u8g2.clearBuffer();
  u8g2.drawFrame(0,0,127,63);
  u8g2.drawLine(0, 18, 127, 18);
  u8g2.setCursor(3, 15);//设置文字显示位置
  u8g2.print("★佛说:");
  u8g2.setCursor(3, 35);
  u8g2.print(textToMe);//将接收到的文字显示出来
  u8g2.sendBuffer();
  delay(8000);
    // Ensure everything is sent

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
//  timer1.run();  
}
