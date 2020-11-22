#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 12, /* cs=*/ 13, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
/*
   BUZZER2
   使用无源蜂鸣器播放《葫芦娃》
*/

//对应音符和频率值
#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112

#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625

//整首曲子的音符部分
int tune[] =
{
  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0,
//  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0, NOTE_D6,
//  NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D6,
//  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0,
//
//  NOTE_D1, NOTE_D1, NOTE_D3,
//  NOTE_D1, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,
//  NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_DH1, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,
//  NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,
//  NOTE_D7, NOTE_D7, NOTE_D5, NOTE_D3,
//  NOTE_D5,
//  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,
//  NOTE_D3, NOTE_D3, NOTE_D1, NOTE_DL6,
//  NOTE_D1,
//  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_DH1, NOTE_D0, NOTE_D7, NOTE_D5,
//  NOTE_D6,
};

//曲子的节拍，即音符持续时间
float duration[] =
{
  1, 1, 0.5, 0.5, 1,
//  0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
//  0.5, 1, 0.5, 1, 0.5, 0.5,
//  0.5, 0.5, 0.5, 0.5, 1, 1,
//
//  1, 1, 1 + 1,
//  0.5, 1, 1 + 0.5, 1,
//  1, 1, 0.5, 0.5, 1,
//  0.5, 1, 1 + 0.5, 1,
//  0.5, 0.5, 0.5, 0.5, 1 + 1,
//  0.5, 1, 1 + 0.5, 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1,
//  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,
//  0.5, 1, 0.5, 1, 1,
//  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,
//  0.5, 1, 0.5, 1, 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1,
//  0.5, 1, 0.5, 1 + 1,
//  0.5, 1, 0.5, 1 + 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1
};

int length;//定义一个变量用来表示共有多少个音符
int tonePin = 15; //蜂鸣器的pin
int echoPin = 5; 
int trigPin = 4; 
int ledPin = 2; 

void music()
{
  for (int x = 0; x < length; x++) //循环音符的次数
  {
    tone(tonePin, tune[x]); //依次播放tune数组元素，即每个音符
    delay(400 * duration[x]); //每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快
    noTone(tonePin);//停止当前音符，进入下一音符
  }
  delay(2000);//等待5秒后，循环重新开始
}

long msToCm(long ms) {
    return ms / 29 / 2; 
}

void ledmatrix() {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFontDirection(1);
//  u8g2.setFont(u8g2_font_victoriabold8_8r);// 设置字母字体
 // u8g2.drawStr(0,8,"I");  // 显示“I”
  u8g2.setFont(u8g2_font_open_iconic_human_1x_t); // choose a suitable font


  for(int i=-8; i<1; i++)
  {
    u8g2.clearBuffer();  
    u8g2.drawGlyph(-1,i,66); 
    u8g2.sendBuffer();
    delay(500);
  }

  for(int j=0; j<6; j++)
  { u8g2.clearBuffer();   
    u8g2.drawGlyph(-1,0,66); // 因为setFontDirection(1)，是在（-1，0）这个坐标开始旋转顺时针90度
    u8g2.sendBuffer();
    delay(200);
  
    u8g2.clearBuffer();   
    u8g2.sendBuffer();
    delay(200);
    
    u8g2.clearBuffer();   
    u8g2.drawGlyph(-1,0,66); // 因为setFontDirection(1)，是在（-1，0）这个坐标开始旋转顺时针90度
    u8g2.sendBuffer();
    delay(1500);
//    for(int k=0; k<2; k++)
//    {u8g2.clearBuffer();   
//    u8g2.drawGlyph(-1,0,66); // 因为setFontDirection(1)，是在（-1，0）这个坐标开始旋转顺时针90度
//    u8g2.sendBuffer();
//    delay(1000);
//  }
 }

//  for(int i=64; i<70; i++){
//  u8g2.drawGlyph(-1,0,i); 
//  u8g2.sendBuffer();
//  delay(1000);
//  }
  
//  u8g2.drawStr(0,7,"A");  // write something to the internal memory
//  u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
//  u8g2.setFont(u8g2_font_open_iconic_play_1x_t);
//  u8g2.setFont(u8g2_font_open_iconic_all_1x_t);  
//  for(int i=64; i<100; i++){

//  u8g2.drawGlyph(0,7,i); 
//  delay(1000);
  u8g2.sendBuffer();
//  }
//  u8g2.sendBuffer();          // transfer internal memory to the display
//  delay(1000);  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8g2.begin();
  pinMode(tonePin, OUTPUT); //设置蜂鸣器的pin为输出模式
  length = sizeof(tune) / sizeof(tune[0]); //这里用了一个sizeof函数，查出数组里有多少个音符
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 

}

void loop() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    long duration_2;
    float cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration_2 = pulseIn(echoPin, HIGH);
    cm = msToCm(duration_2);
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    if (cm > 40) {
        digitalWrite(ledPin, HIGH);
    } else {
        digitalWrite(ledPin, LOW);
        ledmatrix();
        music();

    }
    delay(50);
} 
