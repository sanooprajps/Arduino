/*
Smart Walking
Sound buzzer when the target is nearby
Making use of Ultra sonic sensor
Thinking of having a speaker to talk about the object/distance?!
Ref:-
https://forum.arduino.cc/index.php?topic=193774.0
- Sanoop
*/
int trigPin = 6;
int echoPin = 7;
//int led = A5;
int led = 3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =(duration/2)/29.1;
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println("CM");
  delay(10);
  
//Digital LED test

/*
if((distance<=10)) 
  {
    digitalWrite(led, HIGH);
}
   else if(distance>10)
 {
     digitalWrite(led, LOW);
   }
*/
/*
//Analog LED Test

if((distance<=10)) 
  {
    analogWrite(led, 255);
}
   else if(distance>10)
 {
     analogWrite(led, 0);
   }
*/


if(distance>=100 && distance<=150)
  {
    digitalWrite(led, HIGH);
    //delayMicroseconds(150);
    delay(250);
    digitalWrite(led, LOW);
    //delayMicroseconds(50);
    delay(250);
}

if(distance>=50 && distance<=100)
  {
    digitalWrite(led, HIGH);
    //delayMicroseconds(150);
    delay(50);
    digitalWrite(led, LOW);
    //delayMicroseconds(50);
    delay(50);
}

if(distance>=0 && distance<=50)
  {
    digitalWrite(led, HIGH);
    //delayMicroseconds(100);
    //digitalWrite(led, LOW);
    //delayMicroseconds(175);
}

   else if(distance>100)
 {
    digitalWrite(led, LOW);
}

  //Delay 100ms before next reading.
  delay(100);
  
}
