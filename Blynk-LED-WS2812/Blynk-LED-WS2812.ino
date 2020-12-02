#include <Ethernet.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

#define BLYNK_PRINT Serial

char auth[] = "X2_zrbHwOuJvICNWxCwMsxMfu_bHRlW3";
// Set password to "" for open networks.
char ssid[] = "CMCC-Stone";
char pass[] = "shiyongqian";


int oldState = 1;
//int newState = 1;
int mode     = 0;    // Currently-active animation mode, 0-9

BlynkTimer timer;
//BlynkTimer timer1;

int freq = 20000;    // 频率
int resolution = 8;   // 分辨率 计数位数，取值0 ~ 20（该值决定后面 ledcWrite 方法中占空比可写值，比如该值写10，则占空比最大可写1023 即 (1<<resolution_bits)-1 ） 
int increament = 1;   //变化量
//int delaytime = 40;   //变化延时ms
int channelR = 0;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。
int channelG = 1;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。
int channelB = 2;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。

const int ledR = 25; //RED
const int ledG = 26; //GREEN
const int ledB = 27; //BLUE

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  ledcSetup(channelR, freq, resolution); // 设置通道
  ledcSetup(channelG, freq, resolution); // 设置通道
  ledcSetup(channelB, freq, resolution); // 设置通道
  ledcAttachPin(ledR, channelR);  // 将通道与对应的引脚连接
  ledcAttachPin(ledG, channelG);  // 将通道与对应的引脚连接
  ledcAttachPin(ledB, channelB);  // 将通道与对应的引脚连接

  randomSeed(analogRead(0));
  pinMode(35, INPUT);

  timer.setInterval(1000L, Auto_Random);
}

void loop()
{
  Blynk.run();
  timer.run();  
}


BLYNK_WRITE(V0)
{
  ////Serial.println("-----oldState0-------");
//  Serial.println(newState);
  ////Serial.println(oldState);
  int newState = param.asInt();
  // Get current button state.
 // boolean newState = digitalRead(BUTTON_PIN);
  ////Serial.println("-----newoState1------");
  ////Serial.println(newState);
  ////Serial.println(oldState);
  // Check if state changed from high to low (button press).
  if((newState == 0) && (oldState == 1)) {
    // Short delay to debounce button.
//    delay(5);     //V0虚拟按钮的delay
    //delay(20);   //真实按钮的delay
    // Check if button is still low after debounce.
//    newState = param.asInt();
    ////Serial.println("-----newState2-------");
    ////Serial.println(newState);
    if(newState == 0) {      // Yes, still low
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 1:
          RGB(0,0,255);
 //         delay(1000);
          break;
        case 2:
          RGB(0,255,0);
 //         delay(1000);
          break;
        case 3:
          RGB(255,0,0);
 //         delay(1000);
          break;
        case 4:
          LED_Random();
//          delay(1000);
          break;
        case 5:
          LED_Random();
//          delay(1000);
          break;
        case 6:
          LED_Random();
//          delay(1000);
          break;
        case 7:
          LED_Random();
//          delay(1000);
          break;
        case 8:
          LED_Random();
 //         delay(1000);
          break;
        case 0:
          LED_Random();
 //       delay(10);
          break;
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
//  Serial.println("-----newState3-------");
//  Serial.println(newState);
//  Serial.println(oldState);
}

void Auto_Random(){
  LED_Random();

}

void LED_Random()
{
int analog_value = analogRead(35);
//Serial.println(analog_value);

int randomR = random(255);
int randomG = random(255);
int randomB = random(255);
//Serial.println(randomR);
//Serial.println(randomG);
//Serial.println(randomB);

RGB(randomR,randomG,randomB);
//delay(analog_value+100);
}

void RGB(int R, int G, int B){
  ledcWrite(channelR, R);
  ledcWrite(channelG, G);
  ledcWrite(channelB, B);
}
