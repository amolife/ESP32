#include <Arduino.h>

int freq = 20000;    // 频率
int resolution = 8;   // 分辨率 计数位数，取值0 ~ 20（该值决定后面 ledcWrite 方法中占空比可写值，比如该值写10，则占空比最大可写1023 即 (1<<resolution_bits)-1 ） 
int increament = 1;   //变化量
int delaytime = 40;   //变化延时ms

const int ledR = 25; //RED
const int ledG = 26; //GREEN
const int ledB = 27; //BLUE

int channelR = 0;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。
int channelG = 1;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。
int channelB = 2;    // 通道 ESP32 的 LEDC 总共有16个路通道（0 ~ 15），分为高低速两组，高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。

void setup()
{
  Serial.begin(9600);
  ledcSetup(channelR, freq, resolution); // 设置通道
  ledcSetup(channelG, freq, resolution); // 设置通道
  ledcSetup(channelB, freq, resolution); // 设置通道
  
  ledcAttachPin(ledR, channelR);  // 将通道与对应的引脚连接
  ledcAttachPin(ledG, channelG);  // 将通道与对应的引脚连接
  ledcAttachPin(ledB, channelB);  // 将通道与对应的引脚连接

  randomSeed(analogRead(0));
  pinMode(35, INPUT);
}

void loop()
{
//  fadeOn(channelR);
//    fadeOn(channelG);
//      fadeOn(channelB);
//      
//  fadeOff(channelR);
//    fadeOff(channelG);
//      fadeOff(channelB);

int analog_value = analogRead(35);
Serial.println(analog_value);

int randomR = random(255);
int randomG = random(255);
int randomB = random(255);
Serial.println(randomR);
Serial.println(randomG);
Serial.println(randomB);


RGB(randomR,randomG,randomB);
delay(analog_value+100);

}


void fadeOn(int channel){
  for (int dutyCycle = 0; dutyCycle <= pow(2,resolution); dutyCycle = dutyCycle + increament)
  {
    ledcWrite(channel, dutyCycle);  // 输出PWM
    delay(delaytime);
  }
}

void fadeOff(int channel){
  for (int dutyCycle = pow(2,resolution); dutyCycle >= 0; dutyCycle = dutyCycle - increament)
  {
    ledcWrite(channel, dutyCycle);  // 输出PWM
    delay(delaytime);
  }
}

void RGB(int R, int G, int B){
  ledcWrite(channelR, R);
  ledcWrite(channelG, G);
  ledcWrite(channelB, B);
}
