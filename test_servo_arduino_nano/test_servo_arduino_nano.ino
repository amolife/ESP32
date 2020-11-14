// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include  <Servo.h>
 
Servo myservo_x;  // create servo object to control a servo 
Servo myservo_y;
 
int potpin_x = A1;  // analog pin used to connect the potentiometer
int val_x;    // variable to read the value from the analog pin 

int potpin_y = A2;  // analog pin used to connect the potentiometer
int val_y;    // variable to read the value from the analog pin 

  bool sw;
 
void setup() 
{ 
  myservo_x.attach(7);  // attaches the servo on pin 9 to the servo object 
  myservo_y.attach(8); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(11, INPUT);

} 
 
void loop() 
{ 

  val_x = analogRead(potpin_x);        // reads the value of the potentiometer (value between 0 and 1023) 
  val_x = map(val_x, 0, 1023, 0, 179);   // scale it to use it with the servo (value between 0 and 180) 
  myservo_x.write(val_x);             // sets the servo position according to the scaled value 
//  delay(15);                    // waits for the servo to get there 

    val_y = analogRead(potpin_y);        // reads the value of the potentiometer (value between 0 and 1023) 
  val_y = map(val_y, 0, 1023, 0, 179);   // scale it to use it with the servo (value between 0 and 180) 
  myservo_y.write(val_y);             // sets the servo position according to the scaled value 
 // delay(15);                    // waits for the servo to get there 

    sw = ! digitalRead(11);
  digitalWrite(LED_BUILTIN, sw); 
 //   delay(15); 

  
}
