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
int relayPin = 8;
 
void setup()
{
pinMode (relayPin, OUTPUT);
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
// Print a message to the LCD.
lcd.print("Smart Water Tank!");
// Initialize ASK Object
rf_driver.init();
// Setup Serial Monitor
Serial.begin(9600);
}
 
void loop()
{
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
//lcd.setCursor(0, 0);
// print the number of seconds since reset:
//lcd.print(millis() / 1000);
// Set buffer to size of expected message
uint8_t buf[6];
uint8_t buflen = sizeof(buf);
float recvalue;
// Check if received packet is correct size
if (rf_driver.recv(buf, &buflen))
{
    // Message received with valid che-cksum
    // Convert string to float
    float recvalue = atof((char*)buf);
    Serial.print("Message Received: ");
    //Serial.println((char*)buf);
    Serial.println(recvalue);
    lcd.setCursor(0, 1);
    lcd.print("Normal   :");    
    lcd.setCursor(9,1);  
    
    for (i = 0; i < buflen; i++)
      { 
        lcd.write(buf[i]); 
                    
      }
        
      // lcd.print((char)223);
      if (recvalue < 10.00)
    {
        lcd.setCursor (0,1);
        lcd.print ("Stopping!");
    }
    else if (recvalue > 50.00)
    {
        lcd.setCursor (0,1);
        lcd.print ("Running!");
        digitalWrite (relayPin, HIGH);
        delay (1000);
        digitalWrite (relayPin, LOW);
        delay (1000);
      }
  } 
}