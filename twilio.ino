
//______________Setup WIFI Twilio __________________
#include "twilio.hpp"
#define r 16
#define t 17
static const char *ssid = ""; //Replace with network SSID
static const char *password = ""; //Replace with network password

// Values from Twilio (find them on the dashboard)
static const char *account_sid = "";//Replace with Twilio Account SID
static const char *auth_token = "";//Replace with Twilio Authentication token
// Phone number should start with "+<countrycode>"
static const char *from_number = "+"; //Replace with number being sent from

// You choose!
// Phone number should start with "+<countrycode>"
static const char *to_number = "+"; //Replace with number being sent to
static const char *message = ""; //message to change
//String master_number    = "+";
int CPUtemperature = 0;
int GPUtemperature = 0;
int timePlaying = 0;
int resetcount = 0;
int delaytimer = 0;

Twilio *twilio;
const char fingerprint[] = "93 B5 96 8C BC 63 8D A1 D5 AD 1E DA 30 A1 DB 04 73 DC BC 24";
//
////_______________Setup Stepper code _______________
#include "Arduino.h"

void setup() {
  // ___________ Connect to WIFI and send message ______________
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, r, t);
  WiFi.begin(ssid, password);
Serial.println("beginning...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }

  
}

void loop() {
  String s = Serial2.readString();
  int i = s.toInt();
  String response;
  if(delaytimer >= 10000){//Every 10 seconds, reset the timer
    delaytimer = 0;
    resetcount += 1;
  }
  if(resetcount >= 720){//After 2 hours, check
    if(timePlaying >= resetcount*3/4){//If at least 1 click every second for 75% of a 2-hour time period, notify
      //Send Twilio Message
      twilio = new Twilio(account_sid, auth_token);
      delay(10000);
      delaytimer += 1000;
      message = "We have detected that you have been playing intensely for 2 hours. Please take a break.";
      bool success = twilio->send_message(to_number, from_number, message, response);
    }
    if(i >= 90){//If at least 1 click every second for 75% of a 2-hour time period, notify
      //Send Twilio Message
      twilio = new Twilio(account_sid, auth_token, fingerprint);
      delay(1000);
      delaytimer += 1000;
      message = "Your CPU or GPU temperatures are too high. It may be thermal throttling. We caught the temperature at: " + i;
      bool success = twilio->send_message(to_number, from_number, message, response);
    }
    if(GPUtemperature >= 90){//If at least 1 click every second for 75% of a 2-hour time period, notify
      //Send Twilio Message
      twilio = new Twilio(account_sid, auth_token, fingerprint);
      delay(1000);
      delaytimer += 1000;
      message = "GPU temperature is too high. It may be thermal throttling. Try to increase fan speeds or take a break to decrease temperatures.";
      bool success = twilio->send_message(to_number, from_number, message, response);
    }
    else if(i == 0){//If there is no activity for 2 hours, power off
      //Send Twilio Message
      twilio = new Twilio(account_sid, auth_token, fingerprint);
      delay(1000);
      delaytimer += 1000;
      Serial.print("here");
      message = "No activity was detected for 2 hours. Your haptic device has been turned off.";
      bool success = twilio->send_message(to_number, from_number, message, response);
      return;
    }
  }


  
  
   int minValue = 0; 
   int maxValue = 1000; 
//int constrainedValue = constrain(sensorValue, minValue, maxValue);
  
   // int light = map(constrainedValue, minValue, maxValue, 0, 255);
    
    
//   int rotation = 100 //map (constrainedValue, minValue, maxValue, 1000, 300);
//    if(rotation < 1000){ 
//      //clickCount += 1; //add to click count
//
//    }
//    rotation = 100;
//    Serial.println(rotation);
//    if(rotation<=400){ //When player presses too hard              
//      //Turn on LED
//      digitalWrite(ledPin,HIGH);
//3
//      //Send Twilio Message
//      twilio = new Twilio(account_sid, auth_token, fingerprint);
//      delay(1000);
//      delaytimer += 1000;
//      message = "There is too much force being applied to the mouse, please be more gentle.";
//      bool success = twilio->send_message(to_number, from_number, message, response);

//    //____________Setup LED Pin as Output_____________________
//    pinMode(ledPin, OUTPUT);   
//    }
//    else{
//      digitalWrite(ledPin,LOW);
//    }
//    delay(300);
//    delaytimer += 300;
//    //Run motor
//   if (sensorValue>0 & (check==0)){
//      StepsRequired  =  - STEPS_PER_OUT_REV / 2;   
//      steppermotor.setSpeed(rotation);  
//      steppermotor.step(1);
      
   }


  
