#include <LiquidCrystal.h>// include the library code
#include <Wire.h>
#include <Sharer.h> //library for serial connections


//
char array1[16];  //the string to print on the LCD
char array2[16];  //the string to print on the LCD
int tim = 190;  //the value of delay time
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
//
void setup()
{
  
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
  Sharer.init(9600);
}
//
int values[4]; // an array to store the 4 stat


void loop() 
{
   
   String inputString = " $10 $60 $10 $10";//the imported 4 values with symbol in between in between
   Sharer.run();
  Sharer.read();
  String str = Sharer.readString();
  Sharer.flush();
  Serial.print(str);
  inputString = str;
  //char array3[15];
    //sprintf(array3, "%s", str);
   //lcd.print(array3);
  
  //while (Serial.available() > 0) { // check if there's data available on the serial port
    
    //inputString = Serial.readString(); // read the incoming data and store it in a variable
    //inputString = " $20 $60 $20 $20";
    //Serial.print("Received: "); // print a message to the serial monitor
    //Serial.println(inputString); // print the incoming data to the serial monitor
  //}
  
  // Parse and split the input string into 4 substrings using the space character as the delimiter
  String substrings[4];
  int startIndex = inputString.indexOf('$', 0);
  for (int i = 0; i < 4; i++) {
    int spaceIndex = inputString.indexOf('$', startIndex+1);
      substrings[i] = inputString.substring(startIndex+1, spaceIndex);
      startIndex = spaceIndex;

  }

  // Convert each substring to an integer using parseInt() and store the results in the values array
  for (int i = 0; i < 4; i++) {
    values[i] = substrings[i].toInt();
  }

  // Use sprintf() to format the first two integers with a space character in between and concatenate them into the first char array
  sprintf(array1, "%s %d, %s %d", "CPU", values[0], "Temp", values[1]);

  // Use sprintf() to format the last two integers with a space character in between and concatenate them into the second char array
  sprintf(array2, "%s %d, %s %d", "GPU", values[2], "Temp", values[3]);

  int num1 = values[0];
  int num2 = values[1];
  int num3 = values[2];
  int num4 = values[3]; 

  Serial.println(values[3]); //send GPU temp data to twilio for notifications

  // Set cursor to blink if num > 90
  while (num1 > 90 || num2 > 50 || num3 > 90 || num4 > 50) {
    lcd.blink(); // turn on cursor blinking
    delay(500); // wait for 2000ms
    lcd.noBlink(); // turn off cursor blinking
    lcd.setCursor(0,0);  // set the cursor to column 0, line 0
    lcd.print(array1);  // Print a message to the LCD.
   lcd.setCursor(0,1);  // set the cursor to column 0, line 1
    lcd.print(array2);
    delay(2000); // wait for 2000ms
    lcd.setCursor(0,0);  // set the cursor to column 0, line 0
    lcd.clear();
  String str = Sharer.readString();
  // Parse and split the input string into 4 substrings using the space character as the delimiter
  String substrings[4];
  int startIndex = inputString.indexOf('$', 0);
  for (int i = 0; i < 4; i++) {
    int spaceIndex = inputString.indexOf('$', startIndex+1);
      substrings[i] = inputString.substring(startIndex+1, spaceIndex);
      startIndex = spaceIndex;

  }

  // Convert each substring to an integer using parseInt() and store the results in the values array
  for (int i = 0; i < 4; i++) {
    values[i] = substrings[i].toInt();
  }
    num1 = values[0];
    num2 = values[1];
    num3 = values[2];
    num4 = values[3];
      // Use sprintf() to format the first two integers with a space character in between and concatenate them into the first char array
  sprintf(array1, "%s %d, %s %d", "CPU", values[0], "Temp", values[1]);

  // Use sprintf() to format the last two integers with a space character in between and concatenate them into the second char array
  sprintf(array2, "%s %d, %s %d", "GPU", values[2], "Temp", values[3]);
  }

    lcd.print(array1);  // Print a message to the LCD.
   lcd.setCursor(0,1);  // set the cursor to column 0, line 0
    lcd.print(array2);
    delay(2000); // wait for 2000ms
    lcd.setCursor(0,0);  // set the cursor to column 0, line 0

    num1 = values[0];
    num2 = values[1];
    num3 = values[2];
    num4 = values[3];

}
//
