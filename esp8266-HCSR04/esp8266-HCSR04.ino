int echoPin = 5; 
int trigPin = 4; 
int ledPin = 2; 
void setup() {
Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); 
} 

void loop() {
    long duration;
    float cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm = msToCm(duration);
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    if (cm > 50) {
        digitalWrite(ledPin, HIGH);
    } else {
        digitalWrite(ledPin, LOW);
    }
    delay(100);
} 
long msToCm(long ms) {
    return ms / 29 / 2; 
}


//const int TrigPin = 4;
//const int EchoPin = 5;
//float cm;
//void setup()
//{
//Serial.begin(9600);
//pinMode(TrigPin, OUTPUT);
//pinMode(EchoPin, INPUT);
//}
//void loop()
//{
////发一个10ms的高脉冲去触发TrigPin
//digitalWrite(TrigPin, LOW);
//delayMicroseconds(2);
//digitalWrite(TrigPin, HIGH);
//delayMicroseconds(10);
//digitalWrite(TrigPin, LOW);
//  
//cm = pulseIn(EchoPin, HIGH) / 58.0; //算成厘米
//cm = (int(cm * 100.0)) / 100.0; //保留两位小数
//Serial.print(cm);
//Serial.print("cm");
//Serial.println();
//delay(500);
//}
