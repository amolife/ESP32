/*
   For sis 智能浇花
*/
//#define BLYNK_PRINT Serial
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
//#include <Arduino.h>
//#include <U8g2lib.h>
//#include <Wire.h>
//#include <HTTPClient.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h> 
char auth[] = "n_wxy_ZAYHSP8na1Qw0l3DjORToBIvhn";
char ssid[] = "CMCC-Stone";
char pass[] = "shiyongqian";
int BH1750address = 0x23;
byte buff[2];
uint16_t val = 0;
char str[6];
char setting[7];//远程 本地 显示
//电容式的据网上说空气中560左右，水中310左右的读数
//https://blog.csdn.net/weixin_41866783/article/details/109292153
#define Mud1   34
#define Mud2   35
#define Mud3   32
int Mud1_value;
int Mud2_value;
int Mud3_value;
//电容式V2.0的据网上说空气中560左右，水中310左右的读数,实际校准下实施
//https://blog.csdn.net/weixin_41866783/article/details/109292153
int lowlimit = 1200;
int highlimit = 3800;

int lowrange = 1600;
int highrange = 3500;

//int lowlimit = 310;
//int highlimit = 560;
const int sampleCount = 10;
//int raw = 0; 
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8
#define LED_PIN   14
#define DHTPIN 4          // DHT GPIO
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
int remote = 0;
//本地 远程
int incomedate = 0;
int relayPin = 13;

int duration = 24; //假设多少秒，，以后改为小时


//bool  isWatered = false;    //一段时间内最多浇一次水,防止因为传感器损坏而过浇
//long  waterInerval = 43200000; //正常浇水时间间隔 12*60*60*1000
//long  safeWaterInterval = 86400000;  //安全浇水时间间隔,默认一天最多浇一次水 24*60*60*1000



//继电器引脚
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, 
/* reset=*/
U8X8_PIN_NONE);
//使用硬件IIC
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
BlynkTimer timer;
//********************************
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval((long)duration*1000*60, PumpToDo);
  timer.setInterval(5000L, UpdateData);
  pinMode(relayPin, OUTPUT);
  pinMode(Mud1, INPUT);
  pinMode(Mud2, INPUT);
  pinMode(Mud3, INPUT);
  //analogReference(EXTERNAL); //修改ADC基准电压为外部3.3V
  //adcAttachPin(Mud1);//将引脚连接到ADC
  //adcStart(Mud1);//在连接的引脚总线上开始ADC转换
  //analogReadResolution(16);//设置aliogRead返回值的分辨率
  //****** dht
  dht.begin();
  //******BH1750
  Wire.begin();
  BH1750_Init(BH1750address);
  //******u8g2
  u8g2.begin();
  //u8g2.setFont(u8g2_font_fur20_tf);
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  //******NeoPixel
  strip.begin();
  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();
  // Turn OFF all pixels ASAP
  strip.setBrightness(100);
  // Set BRIGHTNESS to about 1/5 (max = 255)
  rainbow(10);

}
//********************************
void loop() {
  Blynk.run();
  timer.run();
}
//********************************
int MoisureDataClean(int raw_pin) {
  int sum = 0;
  int value1 = 0;
  int raw = 0;
       Serial.printf("..............RawPin(%d)!...............:\n",raw_pin);
  for (int i=0; i < sampleCount; i++) {
    value1 = analogRead(raw_pin);
    delay(1);
    Serial.printf("#(%d):",i);
    Serial.print(value1);
   // Serial.print("\n");
    sum += value1;
  }
  raw = sum / sampleCount;
  //电容式的据网上说空气中560左右，水中310左右的读数
  //https://blog.csdn.net/weixin_41866783/article/details/109292153
  if (raw >= highlimit) {
     Serial.print("\n");
    Serial.printf("数据异常！平均值超过 %d 传感器故障, 平均值是：",highlimit);
     Serial.print(raw);
    return NULL;
  }
  if (raw <= lowlimit) {
     Serial.print("\n");
    Serial.printf("数据异常！平均值低于 %d 传感器故障平均值是：",lowlimit);
         Serial.print(raw);
    return NULL;
  } else {
    raw = map(raw, highrange, lowrange, 0, 100);
     Serial.print("\n");
    Serial.printf("MudValueCleanTo_0_100=%d\n",raw);
    return raw;}

}
//********************************
int BH1750_Read(int address)
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}
//********************************
void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}
//********************************
void rainbow(int wait) {    //彩虹圈
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 1*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


BLYNK_WRITE(V20) // 时间间隔设定
{
duration = param.asInt(); 


}


//void changeFlag(){
//  isWatered = false;  
//}
//void SetTimer(){
//       timer.setInterval((long)duration*1000, PumpToDo);
//}

//********************************
BLYNK_WRITE(V0) //远程就地切换
{
remote = param.asInt(); 
//if(remote == 0){
//  setting = "本地"
//  }
//if(remote == 1){
//  setting = "远程"
//  }
}
//********************************
BLYNK_WRITE(V1) //远程启动停止水泵
{
    incomedate = param.asInt();
//if(remote == 1）{
    if(incomedate == 1 && remote == 1)
    {
      digitalWrite(relayPin, HIGH);
      Serial.println("Pump Run for 3 seconds! From Remote Control");
      delay(3000);
      digitalWrite(relayPin, LOW);
    } 
    if(incomedate == 0 && remote == 1)
    {
      digitalWrite(relayPin, LOW);
      Serial.println("Pump  Stop!  From Remote Control");
     } 
  }
//}
  //if (Serial.available() > 0)//串口接收到数据
  //{
    //incomedate = Serial.read();//获取串口接收到的数据
    //if (incomedate == 'H')
    //{
      //digitalWrite(relayPin, HIGH);
     // Serial.println("Pump Run!");
    //} 
    //else if (incomedate == 'L')
   // {
  //    digitalWrite(relayPin, LOW);
 //     Serial.println("Pump  Stop!");
//    }

//********************************
void PumpToDo()
{
//  Mud1_value = analogRead(Mud1);
//  delay(10);
//  Mud2_value = analogRead(Mud2);
//  delay(10);
//  Mud3_value = analogRead(Mud3);
//  delay(10);
//  Serial.printf("MudSensor(%d)=%d\n",Mud1,Mud1_value);
//  Serial.printf("MudSensor1 Raw=%d\n",Mud1_value);
//  Serial.printf("MudSensor2 Raw=%d\n",Mud2_value);
//  Serial.printf("MudSensor3 Raw=%d\n",Mud3_value);
//  delay(10);

  Mud1_value =MoisureDataClean(Mud1);  //取平均值，清理数据，排除传感器故障情况，并map到0-100
  delay(10);
  Mud2_value =MoisureDataClean(Mud2);
  delay(10);
  Mud3_value =MoisureDataClean(Mud3);
  delay(10);

if(remote == 0){
  if (Mud1_value < 30 && Mud2_value < 30 && Mud3_value < 30)  //三个都太干了 
    {
      digitalWrite(relayPin, HIGH);
      Serial.println("All sensor are too try, so Pump Run!...............");
      delay(5000);
      digitalWrite(relayPin, LOW);
      Serial.println("Planting finished, Pump Stopped!");


     }
  else if (Mud1_value > 75 || Mud2_value > 75 || Mud3_value >75) //有一个太湿了
    {
      digitalWrite(relayPin, LOW);
      Serial.println("One of three sensor is wet so Pump  No need to start pump, wait wait!");
    }
  

}
   
}




void UpdateData()
{

  Mud1_value =MoisureDataClean(Mud1);  //取平均值，清理数据，排除传感器故障情况，并map到0-100
  delay(10);
  Mud2_value =MoisureDataClean(Mud2);
  delay(10);
  Mud3_value =MoisureDataClean(Mud3);
  delay(10);

Blynk.virtualWrite(V11, Mud1_value);
Blynk.virtualWrite(V12, Mud2_value);
Blynk.virtualWrite(V13, Mud3_value);
Blynk.virtualWrite(V20, duration);



Serial.print("Duration=");
Serial.println(duration);
  
  h = dht.readHumidity();
  t = dht.readTemperature(); 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    //return;     ////应该解决了DHT11煞笔以后，系统异常问题
  }
  //rainbow(30);
  if (2 == BH1750_Read(BH1750address))
  {
    if (buff[0] == 255 && buff[1] == 255)
    {
      val = 65535;
    } else {
      val = ((buff[0] << 8) | buff[1]) / 1.2; //芯片手册中规定的数值计算方式
    }
  }

  //u8g2.clearBuffer();
  strcpy(str, u8x8_u16toa(val, 5));  
      Serial.println("光照（Lux）：");
    Serial.println(str);
  u8g2.firstPage();
  do {
//-------------line1--------------   
  u8g2.setCursor(0, 13);
  u8g2.print("土壤:");
  u8g2.setCursor(35, 13);
//  u8g2.print("①:");
//  u8g2.setCursor(40, 13);
  u8g2.print(Mud1_value);   
  u8g2.setCursor(55, 13);
  u8g2.print("%");
  u8g2.setCursor(70, 13);
//  u8g2.print("②:");
//  u8g2.setCursor(80, 13);
  u8g2.print(Mud2_value);   
  u8g2.setCursor(90, 13);
  u8g2.print("%");
  u8g2.setCursor(100, 13);
//  u8g2.print("③:");
//  u8g2.setCursor(120, 13);
  u8g2.print(Mud3_value);   
  u8g2.setCursor(120, 13);
  u8g2.print("%");
//-------------line2--------------
  u8g2.setCursor(0, 29);
  u8g2.print("温:");
  u8g2.setCursor(20, 29);
  u8g2.print(t,1);   // 0.00 to 0.0
  u8g2.setCursor(48, 29);
  u8g2.print("°C");
  u8g2.setCursor(65, 29);
  u8g2.print("湿:");
  u8g2.setCursor(85, 29);
  u8g2.print(h,1);   // 0.00 to 0.0
  u8g2.setCursor(113, 29);
  u8g2.print("%");
//-------------line3--------------
  u8g2.setCursor(0, 45);
  u8g2.print("光照:");
  u8g2.setCursor(30, 45);
  u8g2.print(val);   
  u8g2.setCursor(65, 45);
  u8g2.print("Lux");
  u8g2.setCursor(100, 45);
  u8g2.print(remote);
  u8g2.setCursor(110, 45);
  u8g2.print(incomedate);
//-------------line4--------------
  u8g2.setCursor(0, 61);
  u8g2.print("IP:");
  u8g2.setCursor(20, 61);
  u8g2.print(WiFi.localIP());
//  u8g2.sendBuffer();
  u8g2.setCursor(110, 61);
  u8g2.print(duration);
     
  u8g2.setContrast(255 - map(val, 0, 65535, 0, 255)); //将光照数据进行区间映射，控制OLED背光
     
//    u8g2.drawStr(0, 16, t);
//    u8g2.drawStr(20, 16, "℃");
//    u8g2.drawStr(40, 16, h);
//    u8g2.drawStr(60,16, "%");
//    u8g2.drawStr(80, 16, remote);
//    u8g2.drawStr(0, 32, Mud1_value);
//    u8g2.drawStr(30, 32, Mud2_value);
//    u8g2.drawStr(60, 32, Mud3_value);
     //u8g2.drawStr(10, 48, str);
    //u8g2.drawStr(95, 48, "Lx");

  } while ( u8g2.nextPage() );

   Serial.printf("远程开泵=%d\n",incomedate);
   Serial.printf("远程模式或本地模式=%d\n",remote);

Led();
  
  }

void Led(){

int sum=0;
int avr=0;
sum = Mud1_value + (Mud2_value + Mud3_value);
avr = sum / 3;
Serial.print("3个传感器平均值");
Serial.print(avr);
strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  strip.show();            // Turn OFF all pixels ASAP

if(avr <= 12){
  for(int i=0; i<1; i++) {
  strip.setPixelColor(i, strip.Color(255,   0,   0)); 
  for(int j=1; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >12 && avr <= 25){
  for(int i=0; i<2; i++) {
  strip.setPixelColor(i, strip.Color(255,   0,   0)); 
    for(int j=2; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >25 && avr <= 37){
  for(int i=0; i<3; i++) {
  strip.setPixelColor(i, strip.Color(0,   255,   0)); 
    for(int j=3; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >37 && avr <= 50){
  for(int i=0; i<4; i++) {
  strip.setPixelColor(i, strip.Color(0,   255,   0)); 
    for(int j=4; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >50 && avr <= 62){
  for(int i=0; i<5; i++) {
  strip.setPixelColor(i, strip.Color(0,   255,   0)); 
    for(int j=5; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >62 && avr <= 75){
  for(int i=0; i<6; i++) {
  strip.setPixelColor(i, strip.Color(0,   255,   0)); 
    for(int j=6; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >75 && avr <= 87){
  for(int i=0; i<7; i++) {
  strip.setPixelColor(i, strip.Color(0,   0,   255)); 
    for(int j=7; j<8; j++) {
      strip.setPixelColor(j, strip.Color(0,   0,   0)); 
  }
  strip.show();}
}
else if(avr >87 && avr <= 100){
  for(int i=0; i<8; i++) {
  strip.setPixelColor(i, strip.Color(0,   0,   255)); 
 
  strip.show();}
}

//strip.show();

}
