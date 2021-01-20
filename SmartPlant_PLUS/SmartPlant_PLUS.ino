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
int lowlimit = 200;
int highlimit = 700;
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
  timer.setInterval(5000L, GetSensorAndToDo);
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
}
//********************************
void loop() {
  Blynk.run();
  timer.run();
}
//********************************
int MoisureDataClean(int raw_pin) {
  int sum = 0;
  int val = 0;
  int raw = 0;
       Serial.printf("..............RawPin(%d)!...............:\n",raw_pin);
  for (int i=0; i < sampleCount; i++) {
    val = analogRead(raw_pin);
    Serial.printf("Sample Moisture Sensor Value(%d):",i);
    Serial.print(val);
    Serial.print("\n");
    sum += val;
  }
  raw = sum / sampleCount;
  //电容式的据网上说空气中560左右，水中310左右的读数
  //https://blog.csdn.net/weixin_41866783/article/details/109292153
  if (raw >= 700) {
    Serial.println("数据异常！平均值超过700了，传感器故障");
    return NULL;
  }
  if (raw <= 200) {
    Serial.println("数据异常！平均值低于200了，传感器故障");
    return NULL;
  } else {
    raw = map(raw, lowlimit, highlimit, 0, 100);
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
void GetSensorAndToDo()
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
  delay(100);
  Mud2_value =MoisureDataClean(Mud2);
    delay(100);
  Mud3_value =MoisureDataClean(Mud3);
    delay(100);
if(remote == 0){
  if (Mud1_value > 75 && Mud2_value > 75 && Mud3_value >75)  //三个都太干了 
    {
      digitalWrite(relayPin, HIGH);
      Serial.println("All sensor are too try, so Pump Run!...............");
      delay(5000);
      digitalWrite(relayPin, LOW);
      Serial.println("Planting finished, Pump Stopped!");
     }
  else if (Mud1_value < 30 || Mud2_value < 30 || Mud3_value < 30) //有一个太湿了
    {
      digitalWrite(relayPin, LOW);
      Serial.println("One of three sensor is wet so Pump  No need to start pump, wait wait!");
    }
}
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
    Serial.println(str);
  u8g2.firstPage();
  do {
  u8g2.setCursor(0, 16);
  u8g2.print(t);
  u8g2.setCursor(15, 16);
  u8g2.print("°C");
//    u8g2.drawStr(0, 16, t);
//    u8g2.drawStr(20, 16, "℃");
//    u8g2.drawStr(40, 16, h);
//    u8g2.drawStr(60,16, "%");
//    u8g2.drawStr(80, 16, remote);
//    u8g2.drawStr(0, 32, Mud1_value);
//    u8g2.drawStr(30, 32, Mud2_value);
//    u8g2.drawStr(60, 32, Mud3_value);
    u8g2.drawStr(10, 48, str);
    u8g2.drawStr(95, 48, "Lx");
    u8g2.setContrast(255 - map(val, 0, 500, 0, 255)); //将光照数据进行区间映射，控制OLED背光
  } while ( u8g2.nextPage() );

      Serial.printf("远程开泵=%d\n",incomedate);
            Serial.printf("远程模式或本地模式=%d\n",remote);
}
