/*
RF Transmitter for Automated Water Pump Controller
Ultrasonic Sensor is used to detect the amount of water in the water tank
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
// constants won't change
const int TRIG_PIN = 6; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 7; // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int LED_PIN  = 3; // Arduino pin connected to LED's pin
const int DISTANCE_THRESHOLD = 50; // centimeters
char msg[6];


// variables will change:
float duration, distance;

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
    pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode
    pinMode(LED_PIN, OUTPUT);  // set arduino pin to output mode
}
void loop()
{

  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
   // measure duration of pulse from ECHO pin
  duration = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance = 0.017 * duration;

  if(distance < DISTANCE_THRESHOLD)
    digitalWrite(LED_PIN, HIGH); // turn on LED
  else
    digitalWrite(LED_PIN, LOW);  // turn off LED

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  dtostrf(distance, 6,2,msg); //converts the float into a char 

  delay(500);

  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
    {
      // Message Transmitted
      Serial.println("Message Transmitted: ");
      delay(1000);
  }
}
