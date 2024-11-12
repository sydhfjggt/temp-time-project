/*
* Name: clock and temp project
* Author: Julius Markensten
* Date: 2024-10-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries

// Init constants

// Init global variables

// construct objects
#include "U8glib.h"
#include <RTClib.h>
#include <Wire.h>
#include "Servo.h"

int potpin = 6;
int val;
int i;

RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myservo;

void setup() {
  // init communication

  // Init Hardware
  for( i = 4; i < 6; i++) 
  { 
      val = analogRead(potpin);            
      val = map(val, 0, 1023, 0, 180);     
      myservo.write(val);                  
      delay(15);
  }
 Serial.begin(9600);
 Wire.begin();
 rtc.begin();
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 u8g.setFont(u8g_font_unifont);
 myservo.attach(6);
 //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
}

void loop() {
  oledWrite(getTime(),"Temp: " + String(getTemp()) + " C");
  Servowrite(getTemp());
  delay(1000);
  Serial.println(getTemp());
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime(){

DateTime now = rtc.now();

return ("Time: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));

}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/

float getTemp(){
  int Vo;
  float R1 = 10000;
  float logR2;
  float R2;
  float T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
  
  
  Vo = analogRead(A1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); // temperature in Kelvin
  T = T - 273.15;


  return T;
}
  

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text, String temp){

 u8g.firstPage();
  do {
    u8g.drawStr(5, 20, text.c_str());
    u8g.drawStr(5, 40, temp.c_str());

  } while (u8g.nextPage());

}

/*
* takes a temprature value and maps it to corresponding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void Servowrite(float value){

  myservo.write(map(getTemp(), 0, 25, 0, 180));

}