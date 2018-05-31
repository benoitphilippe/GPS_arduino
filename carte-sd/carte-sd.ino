#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "GPS.h"
#include "journey.h"

// MAX_BATTERY
#define MAX_BATTERY 774.0f
// We are using a board which uses the pin 10/SS for the SD card (CS/SS pin)
#define SDPORT 10
// Initailisation Time
#define INITTIME 3000

// lcd printer
LiquidCrystal lcd(4,5,6,7,8,9);

// GPS serial and device
TinyGPS gps;
SoftwareSerial ss(3, 2);

// GPS global datas, filled with getGPSData()
float flat, flon;
float l_distance; // distance in metters
unsigned long age;
unsigned long time, date;
Journey* ptr_journey;

void setup() {
  Serial.begin(9600);
  lcd.begin(8,2);
  ss.begin(4800);
  while(!Serial) {
    ;
  }
  lcd.print("Battery");
  lcd.setCursor(0,1);
  lcd.print(((float)analogRead(A0) * 100.0f)/MAX_BATTERY);
  lcd.print('%');

  if (!SD.begin(SDPORT)) {
    Serial.println("initialization SD failed!");
    while (1);
  }

  l_distance = 0.0f;  // distance initialisation
  flat = 0.0f; flon = 0.0f; // position initialisation

  // Journey test
  ptr_journey = new Journey(5);
  ptr_journey->start_recording();
}

void loop(void) {
  if (getGPSData())
    { // when new datas are available
        lcd.setCursor(0,0);
        lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        lcd.setCursor(0,1);
        lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    }
}
