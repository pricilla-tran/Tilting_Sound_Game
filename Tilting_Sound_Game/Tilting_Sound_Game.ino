// Library for the Sensor Kit 
#include "Arduino_SensorKit.h"


boolean finish, finish2 = false;
#define potentiometer A0
#define BUZZER 5
#define LED 2
#define LED2 6


float randTilt;
int randValue;
long randPause;
long randFreq;
bool win = false;

void setup() {
  
  Serial.begin(9600);

  pinMode(potentiometer, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
 
  while(!Serial);
  Accelerometer.begin();

  
  Oled.begin();
  Oled.setFlipMode(true);
   
  randPause = random(1,500);
  randTilt = random(100);
  
}

void loop() {
  
  // Code for Raspberry Pi 
  if (Serial.available() > 0) {
    String piCommand = Serial.readStringUntil('\n');
    if (piCommand == "Playing") {
      Serial.print("Playing");
      Serial.println();

      // Potentiometer
      int sensorValue = analogRead(potentiometer);
      
      // Accelerometer
      double tiltValue = ((Accelerometer.readX() + Accelerometer.readY() + Accelerometer.readZ())*100);
      int value = map(sensorValue, 0, 1023, 0, 500);

      // Conditions for the LED for the Potentiometer Value 
      if(randPause > (value-5) && randPause < value || randPause < (value+5) && randPause > value){
        digitalWrite(LED2, HIGH);
        delay(100);
        finish = true;
      } else if (randPause < (value+50) && randPause > value || randPause > (value-50) && randPause < value){
        digitalWrite(LED2, HIGH);
        delay(50);
        digitalWrite(LED2, LOW);
        delay(50);
        digitalWrite(LED2, HIGH);
        delay(50);
        digitalWrite(LED2, LOW);
        delay(50);
      } else {
        digitalWrite(LED2, LOW);
      }   

      // Conditions for the LED for the Accelerometer Value 
      if(randTilt < (tiltValue+10) && randTilt > tiltValue || randTilt > (tiltValue - 10) && randTilt < tiltValue){
        digitalWrite(LED, HIGH);
        delay(100);
        finish2 = true;
      } else if (randTilt < (tiltValue+50) && randTilt > tiltValue || randTilt > (tiltValue-50) && randTilt < tiltValue){
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        delay(50);
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        delay(50);
      } else {
        digitalWrite(LED, LOW);
      }  

      // Values controlling the sound from the buzzer (piezo)
      tone(BUZZER, (tiltValue)); 
      delay(sensorValue);
      noTone(BUZZER);
      delay(sensorValue);

      // Win Condition including Pi Operation 
      if (finish == true && finish2 == true){
        win = true;
        noTone(BUZZER);
        Serial.print("Win");
        Serial.println();
        //exit(0);
      }
    } 

    // Celebration when the Puzzle Box is completed
    else if (piCommand == "Celebrate") {
      double tiltValue = ((Accelerometer.readX() + Accelerometer.readY() + Accelerometer.readZ())*100);
      for (int i = 0; i < 6; i++) {
        tone(BUZZER, (int)tiltValue/i);
        digitalWrite(LED, HIGH);
        digitalWrite(LED2, HIGH);
        delay(10);
        digitalWrite(LED, LOW);
        digitalWrite(LED2, LOW);
      }
      noTone(BUZZER);
    }
  }
}
