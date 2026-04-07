/* File: arduinocode_draft1_mar15 
* Author: Edie / Kip / Morgan 
* Date: 2026/03-16
* Description: This file contains a program that will read the
* analog inputs for the SOLAR PLANETARIUM and convert these values 
* into a number from 0-1000 to represent # of days in the past or 
* future. These numbers are then sent to the Raspberry Pi 
* using a USB connection. 
*/

#define ANALOG_PASTDATE A2    // pin number for analog input of previous date
#define ANALOG_FUTUREDATE A1  // pin number for analog input of future date
#define STARTBTN 7           // pin number for start button to communicate to pi
#define SCALE(x) (x + 1.47)   // 0-680 input range on analog input scaled to 0-1000 days
#define BAUDRATE 9600         // baud rate for USB communication to the raspberry pi
#define INITIALIZER 0         // initialize variables
#define DELIMITER "!"         // delimiter used to separate numbers on raspberry pi using strtok
#define START "Start"         //message to indicate to pi button has been pressed

void analogReading(int);

/* function: setup 
*  description: sets up the arduino pins as necessary 
*  and begins serial communication. 
* @param: none 
* @return: none 
* side effects: none  */
void setup() {
  pinMode(ANALOG_PASTDATE, INPUT);
  pinMode(ANALOG_FUTUREDATE, INPUT);
  pinMode(STARTBTN, INPUT_PULLUP);
  Serial.begin(BAUDRATE);
}

void analogReading(int pinNum) {
  int pureReading = INITIALIZER;
  float scaleReading = INITIALIZER;
  pureReading = analogRead(pinNum);          // read analog input at designated pin number
  scaleReading = SCALE((float)pureReading);  // convert the reading to a float and scale to 0-1000 range
  Serial.print(scaleReading);                // prints the reading to the raspberry pi via USB connection
  return;
}

/* function: main
*  description: calls functions to read and send analog values to raspberry pi, and sends delimiters for use in raspberry pi program. 
* @param: none 
* @return: none 
* side effects: start button is a momentary pushbutton, in case of 
* unintended operation verify circuit config  */
void loop() {
  if(STARTBTN != HIGH) {
    analogReading(ANALOG_PASTDATE);
    Serial.print(DELIMITER);  // delimiter used in raspberry pi to separate past/present # of days
    analogReading(ANALOG_FUTUREDATE);
    Serial.print('\n');  // terminator used in raspberry pi to end reading of serial input
    delay(1000); 
  }
  else if(STARTBTN == HIGH) {
    analogReading(ANALOG_PASTDATE);
    Serial.print(DELIMITER);  // delimiter used in raspberry pi to separate past/present # of days
    analogReading(ANALOG_FUTUREDATE);
    Serial.print(DELIMITER);
    Serial.print(START);
    Serial.print('\n');  // terminator used in raspberry pi to end reading of serial input
    delay(2000); 
    String indicator = Serial.readString();
    while (indicator != "Hi") {
      indicator = Serial.readString();
    }
  }
}
