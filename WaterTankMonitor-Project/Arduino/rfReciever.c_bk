#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library
#include <LiquidCrystal.h> //LCD display library
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//int i;    //and integer used to count
int i;    //and integer used to count
int relayPin = 8; //Relay to control the motor
int motorFlag = 0; //0: Not running, 1: Running
//Water Tank variables
const float tankCapacity = 750.00; //Value in liter
const float tankHeight = 93.00; //Value in Centimeter

float waterLevel = 0;
//RF Receiver specific config
// Set buffer to size of expected message
uint8_t buf[6];
uint8_t buflen = sizeof(buf);
float f_recvalue, s_recvalue, l_recvalue;

/*
* Function to capture RF data from the Tx. 
* This will be called by both setup and loop.
*/
float captureRfData() {
  delay(2000);
  if (rf_driver.recv(buf, &buflen))
    {
    // Message received with valid che-cksum
    // Convert string to float
    float f_recvalue = atof((char*)buf);
    Serial.print("captureRfData: Tx Message Received: ");
    Serial.println(f_recvalue);
    return f_recvalue;
  }
  else
    {
    Serial.println("captureRfData: No response from Tx");
    //lcd.print ("Function: Tx ERROR!");
    delay (1000);
  }
}

/*
* Fuction checks in a loop to see if sensor value is correct.
* It returns 0 for failure and 1 for success
*/

int confirmSensorLoop(int thresholdVal, char OperatorL[]){
  
  int conf_count = 0;
  int conf_error=0;

   while (conf_count < 5) {
      float conf_recvalue = 00.00;
      conf_recvalue = captureRfData();
      if (conf_recvalue == NULL){
        conf_error++;
        Serial.println("confirmSensorLoop: NULL Value received from Tx");
      }
      if (strcmp(OperatorL, "gt") == 0) {
        if (conf_recvalue > thresholdVal && conf_recvalue != NULL) {
          Serial.print("confirmSensorLoop: Sensor value satisfied for threshold:- ");
        }
        else {
          Serial.print("confirmSensorLoop: invalid sensor value for threshold:- ");
          conf_error++;
        }
      }

      if (strcmp(OperatorL, "lt") == 0) {
        if (conf_recvalue < thresholdVal && conf_recvalue != NULL) {
          Serial.print("confirmSensorLoop: Sensor value satisfied for threshold:- ");
        }
        else {
          Serial.print("confirmSensorLoop: invalid sensor value for threshold:- ");
          conf_error++;
        }
      }
      Serial.println(conf_recvalue);
      conf_count++;
    }

    if (conf_error > 2) {
      Serial.println("confirmSensorLoop: Confirmation condition not satified. Failing Operation!");
      return 0;
    }
    else {
      return 1;
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
int result_setup = 2;
const int totalThreshold = 100; //Total capacity of the tank
lcd.clear();
Serial.println("Setup: Initializing!!");
lcd.print("Initializing!");
static const char Operation_setup[] = "lt";
result_setup = confirmSensorLoop(totalThreshold, Operation_setup);
if (result_setup == 0){
  Serial.println("Setup: Stopping Arduino. Fix the Tx issue and reset Arduino.");
  lcd.clear();
  lcd.print ("Setup: Tx ERROR!");
  for (;;);
}
Serial.print("Setup: Successfully initialized!\n\n");
lcd.clear();
lcd.print("All good! :)");
delay(1000);
}
 
void loop()
{
l_recvalue = captureRfData();
// Calculate Water tank Capacity
waterLevel = tankCapacity - ((tankCapacity/tankHeight) * l_recvalue);
Serial.print ("loop: Water Level:- ");
Serial.print (waterLevel);
Serial.print ("L \n");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Water :- ");
static char s_waterLevel[5];
lcd.setCursor(9,0);
dtostrf(waterLevel, 5,2,s_waterLevel);
lcd.write(s_waterLevel);
lcd.setCursor(15,0);
lcd.print("L");
int result_loop = 2;
const int lowThreshold = 70; //When the tank is empty, more distance to tank
const int highThreshold = 20; //When the tank is full, less distance to reach water

if (l_recvalue < highThreshold) {
  if (motorFlag == 1) {
    static const char Operation_loop_ht[] = "lt";
    result_loop = confirmSensorLoop(lowThreshold, Operation_loop_ht); 
  }
  if (result_loop == 1 && motorFlag == 1) {
    lcd.setCursor (0,1);
    lcd.print ("Stopping!");
    Serial.print("loop: Stopping the motor as it reached 10cm close.\n");
    digitalWrite (relayPin, LOW);
    motorFlag = 0;
  }
  else if (motorFlag == 0) {
    Serial.println("loop: Continue keeping the motor idle as the distance is  <20cm");
  }
  else {
    Serial.println("loop: False sensor value. Continue in the same state.");
  }
}
else if (l_recvalue > lowThreshold){
  if (motorFlag == 0) {
    static const char Operation_loop_lt[] = "gt";
    result_loop = confirmSensorLoop(lowThreshold, Operation_loop_lt);
  }
  if (result_loop == 1 && motorFlag == 0) {
    lcd.setCursor (0,1);
    lcd.print ("Motor Running!");
    Serial.print("loop: Starting the motor as it reached 70cm long\n");
    digitalWrite (relayPin, HIGH);
    motorFlag = 1;
  }
  else if (motorFlag == 1) {
    Serial.println("loop: Continue running the motor as the distance is still >70cm");
  }
  else {
    Serial.println("loop: False sensor value. Continue in the same state.");
  }
}
else {
  lcd.setCursor (0,1);
  lcd.print ("Normal!");
  Serial.print("loop: Water level is normal. Persisting the previous state!\n");
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
}