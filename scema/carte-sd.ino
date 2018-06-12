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
    while (1);
  }

  l_distance = 0.0f;  // distance initialisation
  flat = 0.0f; flon = 0.0f; // position initialisation

  // Journey test
  ptr_journey = new Journey(4); // create or continue a journey with ID 0 (up to MAX_JOURNEY_IN_MEMORY IDs from journey.h)
  /* if needed, next two lines reset the journey */
  //ptr_journey->erase_from_memory(); // delete datas (EEPROM and SD) from this journey (ID 0), and reset distance and time
  //ptr_journey->save_on_EEPROM(); // save reseted values on EEPROM
  Journey::print_all_EEPROM(); // print EEPROM status (of all journey)
  ptr_journey->print_coords(); // print coords saved in SD for this journey (must be stop_recording() mode)
  //ptr_journey->start_recording(); // allow update_datas() and append_point()
}

void loop(void) {
  static int count = 0;
  if (getGPSData())
    { // when new datas are available
      lcd.setCursor(0,0);
      lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      lcd.setCursor(0,1);
      lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      count++;
      
      /** Each time GPS get new datas, call update_datas() and append point(). don't remove those function
       *  These fonctions won't do anything if start_recording() was not called or if stop_recording() was called
       */ 
      ptr_journey->update_datas(); // update distance and time, do nothing without start_recording()
      ptr_journey->append_point(); // add new point in SD file, do nothing without start_recording()


      if(count == 10){ // system D for stop record
        Serial.println(ptr_journey->get_total_distance());
        Serial.println(ptr_journey->get_time());
        //ptr_journey->stop_recording(); // stop recording
        //ptr_journey->save_on_EEPROM(); // save new distance and time of journey in EEPROM /!\ do not call it too often
        //delete ptr_journey; // free pointer /!\ stop recording must be called first
        count = 0;
      }

    }
}
