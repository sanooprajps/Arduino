#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library
#include <LiquidCrystal.h> //LCD display library
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i;    //and integer used to count
int relayPin = 8; //Relay to control the motor
int motorFlag = 0; //0: Not running, 1: Running
//Water Tank variables
const float tankCapacity = 750.00; //Value in liter
const float tankHeight = 93.00; //Value in Centimeter
float waterLevel=0;
//RF Receiver specific config
// Set buffer to size of expected message
uint8_t buf[6];
uint8_t buflen = sizeof(buf);
float f_recvalue, s_recvalue, l_recvalue;

// Function to capture RF data from the Tx. This will be called by both setup and loop.
float captureRfData() {
  delay(2000);
  if (rf_driver.recv(buf, &buflen))
    {
    // Message received with valid che-cksum
    // Convert string to float
    float f_recvalue = atof((char*)buf);
    Serial.print("Function: Tx Message Received: ");
    Serial.println(f_recvalue);
    return f_recvalue;
  }
  else
    {
    Serial.print("Function: Invalid message received from Tx\n");
    //lcd.print ("Function: Tx ERROR!");
    delay (1000);
  }
}

// Setup function
void setup()
{
digitalWrite (relayPin, LOW);
pinMode (relayPin, OUTPUT);
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
// Print a Welcome Message to the LCD.
//lcd.print("Welcome!!");
//delay(2000);
// Initialize ASK Object
rf_driver.init();
// Setup Serial Monitor
Serial.begin(9600);
// Loop to check whether Trasmitter is sending data continuously
int wcount = 0;
int errcount = 0;
lcd.clear();
Serial.print("Setup: Initializing!!\n");
lcd.print("Initializing!");
while (wcount < 5){
  Serial.print("\n");
  Serial.print("Setup: Inside While:");
  Serial.print(wcount);
  Serial.print("\n");
  //delay (2000);
  s_recvalue = captureRfData();
  if (s_recvalue == NULL){
    errcount++;
    Serial.print ("Setup: NULL Value received from Tx\n");
  }
  if (s_recvalue < 100.00 ){
    Serial.print("Setup: Tx Message Received: :-");
    Serial.print(s_recvalue);
    Serial.print("\n");
  }
  else {
    Serial.print("Setup: ERROR!. Received invalid data from the Transmitter\n");
    Serial.print (s_recvalue);
    Serial.print ("Stopping Arduino at Setup stage. Fix the transmitter issue and reset Arduino.\n");
    errcount++;
    if (errcount > 2) {
      lcd.clear();
      lcd.print ("Setup: ERROR!");
      for (;;);
    }
  }
  wcount++;
}
Serial.print("Setup: Successfully initialized!\n\n");
lcd.clear();
lcd.print("All good! :)");
delay(1000);
}
 
void loop()
{
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
//lcd.setCursor(0, 0);
// print the number of seconds since reset:
//lcd.print(millis() / 1000);
l_recvalue = captureRfData();
// Calculate Water tank Capacity
waterLevel = (tankCapacity/tankHeight) * l_recvalue;
Serial.print ("loop: Water Level Calculated:- ");
Serial.print (waterLevel);
Serial.print ("Liter \n");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Water Level:- ");
static char s_waterLevel[5];
lcd.setCursor(13,0);
dtostrf(waterLevel, 5,2,s_waterLevel);
lcd.write(s_waterLevel);

if (l_recvalue < 20) {
  lcd.setCursor (0,1);
  lcd.print ("Stopping!");
  Serial.print("loop: Stopping the motor as it reached 10cm close.\n");
  digitalWrite (relayPin, LOW);
  motorFlag = 0;
}
else if (l_recvalue > 70){
  lcd.setCursor (0,1);
  lcd.print ("Running!");
  Serial.print("loop: Starting the motor as it reached 70cm long\n");
  digitalWrite (relayPin, HIGH);
  motorFlag = 1;
}
else {
  lcd.setCursor (0,1);
  lcd.print ("Normal: Running!");
  Serial.print("loop: Running the motor, and water level is normal\n");
}

if (motorFlag == 1) {
  delay(1000);
}
else if (motorFlag == 0){
  delay (5000);
}
else {
  Serial.print("loop: ERROR:: Something wrong in getting motorStatus.");
  Serial.print(motorFlag);
}

// Check if received packet is correct size
/*
if (rf_driver.recv(buf, &buflen))
{
    // Message received with valid che-cksum
    // Convert string to float
    float recvalue = atof((char*)buf);
    Serial.print("Message Received: ");
    Serial.println(recvalue);
   // lcd.setCursor(0, 1);
    //lcd.print("Normal   :");    
    //lcd.setCursor(9,1);  
    
    for (i = 0; i < buflen; i++)
      { 
        lcd.write(buf[i]); 
                    
      }
        
      // lcd.print((char)223);
      if (recvalue < 10.00)
    {
        lcd.setCursor (0,1);
        lcd.print ("Stopping!");
        Serial.print("Making relay pin HIGH: ");
        digitalWrite (relayPin, HIGH);
        delay (2000);
        digitalWrite (relayPin, LOW);
    }
    else if (recvalue > 50.00)
    {
        lcd.setCursor (0,1);
        lcd.print ("Running!");
        
      }
  }
  else
  {
    Serial.print("Transmitter ERROR!");
    lcd.print ("Transmitter ERROR!");
    delay (1000);
  }
  */
}