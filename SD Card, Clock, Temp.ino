/*
Save temperature in SD/microSD card every hour with DS3231 + SD/microSD module + 
Arduino
Original code derived from: https://electropeak.com/learn/
*/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

File myFile;
DateTime now;
int newMin = 0;
int oldMin = 0;
bool counter = false;

void save_temperature() {
  myFile = SD.open("temp.txt", FILE_WRITE);
  now = rtc.now();

  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(",");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());

  rtc.convertTemperature();  //convert current temperature into registers
  myFile.print(",");

  myFile.println(rtc.getTemperature());  //read registers and save temperature on deg C
  myFile.close();
}

void log() {
  myFile = SD.open("log.txt", FILE_WRITE);
  now = rtc.now();

  if(counter == false) {
  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(" ");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());
  myFile.print(":");
  myFile.print(now.second());
  myFile.println(" System started.");
  }

  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(" ");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());
  myFile.print(":");
  myFile.print(now.second());
  myFile.println(" Reading temperature sensor.");

  rtc.convertTemperature();

  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(" ");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());
  myFile.print(":");
  myFile.print(now.second());
  myFile.print(" Got ");
  myFile.print((int)rtc.getTemperature());
  myFile.println(" degrees C.");

  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(" ");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());
  myFile.print(":");
  myFile.print(now.second());
  if (!SD.begin(10))
    myFile.print("Error writing to the SD card");
  else 
    myFile.println(" Writing value to SD card.");

  myFile.print(now.year());
  myFile.print("-");
  myFile.print(now.month());
  myFile.print("-");
  myFile.print(now.date());
  myFile.print(" ");

  myFile.print(now.hour());
  myFile.print(":");
  myFile.print(now.minute());
  myFile.print(":");
  myFile.print(now.second());
  myFile.println(" Successfully wrote value to SD card.");

  myFile.close();
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  myFile = SD.open("temp.txt", FILE_WRITE);
  myFile.println("Date,Time,Temperature");
  myFile.close();
  now = rtc.now();
  oldMin = now.minute();
}

void loop() {
  now = rtc.now();
  newMin = now.minute();
  if (oldMin != newMin) {
    save_temperature();
    log();
    counter = true;
    oldMin = newMin;
  }
}
