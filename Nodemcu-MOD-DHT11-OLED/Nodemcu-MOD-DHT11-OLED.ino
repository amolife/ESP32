/*************************************************************
  项目说明：读取DHT11并显示在OLED
  App项目设置:
  创建Value Display组件，输入管脚设置为V5,数值范围0-100,reading rate设置为push
  创建Value Display组件，输入管脚设置为V6,数值范围-20-60,reading rate设置为push
  
  硬件连接：
  OLED连接在IIC接口
  DHT11连接在GPIO2管脚

  blynk_for_chinese 库地址：https://github.com/hznupeter/blynk-library-for-chinese

  Blynk物联网学习资料：https://gitee.com/hznupeter/Blynk_IOT/wikis
  *************************************************************/
//  #define BLYNK_PRINT Serial // 开启串口监视
//  #include <ESP8266WiFi.h>
//  #include <BlynkSimpleEsp8266.h>
  #include <DHT.h>
  #include <Arduino.h>
  #include <U8g2lib.h>
  #include <Wire.h>
//  char auth[] = "14b1f8e400b64d53b1c557cad983a114";
//  char ssid[] = "ssid";
//  char pass[] = "pass";
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 4, /* data=*/ 5);
#define DHTPIN 2//传感器连接管脚
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

const uint8_t u8g2_font_ziku[450] U8G2_FONT_SECTION("u8g2_font_ziku") =
"\24\0\3\2\4\4\3\2\6\17\17\0\376\14\376\15\376\0\0\0\0\1\25 \5\0\234\4%\36\334\224"
"\325\246EC\22\205Q\26&bT\226r\60\307\242\65\312\302(\353\24\16Y\16\1\60\16\307=\325\246"
"(\341\352\327-\32\24\0\61\7\263>\305\324\177\62\16\307=\325\246(\251\32\247j\257\303\20\63\22\307"
"=\325\246(\241\252\206:\220\3\261**\203\2\64\20\267=mjL\262(\222j\311\60\244q\5\65"
"\20\267=\325 \245q<\310r\254\212\312\240\0\66\20\267=e\252V\207L\61\252\352\246\14\12\0\67"
"\17\267=\305!\215\323\70\215\323\70\215S\0\70\22\307=\325\246(\341\272E\203\224%\251\243\62(\0"
"\71\20\307=\325\246(\341\252n\321\240\246jU\4H\25\271}\205:\244C:\244\303C:\244C:"
"\244C:\244\2M\36\275\375M\216\352@\246\3\322\232DI\232D\211\26\265EI\246dI\230dI"
"(\256\5P\21\270=\305!\21\23q\10\23M\31\64\331\63\0R\26\270]\305!\21\23q\10\207L"
"\31\64%\224\62\311\246\210\11\0\0\0\0\4\377\377!\3\30\275\35\316\250E\221iQ\353`\216\346h"
"\216\346p\216\252\361\260\0^\246&\376\5>\220\303\71\360!\311\302\312pP\262\260\26\326\206\61\207\223"
"\341\20ei$FZ<F\203\62\354h\0n)*\356\15NNP\206AL\342x\30\344\212\26g"
"\321\60\350\324\341\66$Q\222%J\224D\212\22%Q\244DI\242\14\7\1n\177(\356\15NNP"
"\206A.\17\203\22\306\311\24g\321\60\350H\224&Q/Q\42U\242$\312\222hJ\243L\31\16\2"
"\0";

DHT dht(DHTPIN, DHTTYPE);
//BlynkTimer timer;
void setup(void) {
//  Serial.begin(9600);
//  Blynk.begin(auth, ssid, pass);//官方服务器
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//自建服务器域名模式
  //Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 31, 249), 8080);//自建服务器ip模式
  dht.begin();
//  timer.setInterval(1000L, sendSensor);
  u8g2.begin();
  u8g2.enableUTF8Print();  
}
//void sendSensor()
//{
//  float h = dht.readHumidity();
//  float t = dht.readTemperature(); //摄氏度
//  //float t = dht.readTemperature(true); //华氏度
//  Blynk.virtualWrite(V0, t);//将湿度发送给V0
//  Blynk.virtualWrite(V1, h);//将湿度发送给V1

//}
void loop(void) {
//  Blynk.run();
//  timer.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //摄氏度


  u8g2.setFont(u8g2_font_unifont_t_chinese1); 
//  u8g2.setFont(u8g2_font_ziku);    //网上下载的字库，有“温度，湿度”字体，但是太丑陋
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
//  u8g2.setFontPosTop();
  u8g2.setCursor(0, 15);
  u8g2.print("T:");
  u8g2.setCursor(60, 15);
  u8g2.print(t);
  u8g2.setCursor(0, 40);
  u8g2.print("H:");
  u8g2.setCursor(60, 40);
  u8g2.print(h);
  
  u8g2.sendBuffer();
  delay(5000);
}
