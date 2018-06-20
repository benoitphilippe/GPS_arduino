#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "GPS.h"
#include "journey.h"
#include "Bounce2.h"
#include "menu.h"

// MAX_BATTERY
#define MAX_BATTERY 774.0f
// We are using a board which uses the pin 10/SS for the SD card (CS/SS pin)
#define SDPORT 10

// lcd printer
LiquidCrystal lcd(4,5,6,7,8,9);

// GPS serial and device
TinyGPS gps;
SoftwareSerial ss(3, 2);

// Buttons
#define BP0 16 
#define BP1 15
#define BPEN 17

// Debouncers
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
// if button is pushed 1, else 0
int current_enable;

// GPS global datas, filled with getGPSData()
float flat, flon;
float l_distance; // distance in meters
float falt; // altitude in metters
unsigned long age;
unsigned long time, date;
Journey* ptr_journey;

// Input and menu variables
int input = 0;
Fenetres menu = fen1;
//Keep track of trip chosen by user
int currentTrip;

void setup() {
  // Initialize display and communication
  Serial.begin(9600);
  lcd.begin(8,2);
  ss.begin(4800);
  while(!Serial) {
    ;
  }

  // Initialize button with debouncers
  pinMode(BP0, INPUT_PULLUP);
  debouncer1.attach(BP0);
  debouncer1.interval(5);
  
  pinMode(BP1, INPUT_PULLUP);
  debouncer2.attach(BP1);
  debouncer2.interval(5);

  pinMode(BPEN, INPUT_PULLUP);
  debouncer3.attach(BPEN);
  debouncer3.interval(5);
  
  // Print battery for 3sec
  lcd.print("BATTERY");
  lcd.setCursor(0,1);
  lcd.print(((float)analogRead(A0) * 100.0f)/MAX_BATTERY);
  lcd.print('%');
  delay(3000);
  lcd.clear();
  delay(100);
  if (!SD.begin(SDPORT)) {
    while (1);
  }

  l_distance = 0.0f;  // distance initialisation
  flat = 0.0f; flon = 0.0f; // position initialisation

  // Journey test
  Journey::print_all_EEPROM(); // print EEPROM status (of all journey)
  
}

void loop() {

  // Input & UI
  input = clavier();
  
  if (input != 0) {
    lcd.clear();// clear lcd Screen
    update_menu(); // update menu
    do_action(); // do action from user input
  } 

  // GPS data fetching
  if (getGPSData())
    {      
      /** Each time GPS get new datas, call update_datas() and append point(). don't remove those function
       *  These fonctions won't do anything if start_recording() was not called or if stop_recording() was called
       */ 
      ptr_journey->update_datas(); // update distance and time, do nothing without start_recording()
      ptr_journey->append_point(); // add new point in SD file, do nothing without start_recording()
    }
}
