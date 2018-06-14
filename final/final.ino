#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "GPS.h"
#include "journey.h"
#include "Bounce2.h"

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
enum fenetres {
  trip1, trip2, trip3, trip4, trip5, trip6, trip7, trip8, trip9, trip10, fen1, fen2, fen11, fen12, fen13, fen14, fen32, fen33, fen34, fen42, fen43, fen44, fen45, fen46, fen56
  };
fenetres menu = fen1;
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
  //ptr_journey = new Journey(4); // create or continue a journey with ID 0 (up to MAX_JOURNEY_IN_MEMORY IDs from journey.h)
  /* if needed, next two lines reset the journey */
  //ptr_journey->erase_from_memory(); // delete datas (EEPROM and SD) from this journey (ID 0), and reset distance and time
  //ptr_journey->save_on_EEPROM(); // save reseted values on EEPROM
  Journey::print_all_EEPROM(); // print EEPROM status (of all journey)
  //ptr_journey->print_coords(); // print coords saved in SD for this journey (must be stop_recording() mode)
  //ptr_journey->start_recording(); // allow update_datas() and append_point()

}

void loop() {
  static int count = 0;

  // Input & UI
  input = clavier();
  
  //////////////////
  // MENU DEBUG
  //lcd.setCursor(1,0);
  //lcd.print(((int)menu));
  //lcd.setCursor(0,1);
  //lcd.print(((int)current_enable));
  //lcd.setCursor(7,1);
  //////////////////
  
  if (input != 0) {
    lcd.clear();
      
      switch(input)
    {
      // Button 1 is "cancel" or "return"
      case 1:
        if (menu==fen11|menu==fen12) {
          menu=fen1;
        }
        else if (menu==trip1|menu==trip2|menu==trip3|menu==trip4|menu==trip5|menu==trip6|menu==trip7|menu==trip8|menu==trip9|menu==trip10) {
          menu=fen2;
        }
        // Return to memory slot selection
        else if (menu==fen32|menu==fen33|menu==fen34) {
          // Erasing previous pointer
          delete ptr_journey;
          menu=currentTrip;
          //menu=trip1;
        }
        else if (menu==fen42) {
          menu=fen32;
        }
        else if (menu==fen43|menu==fen44|menu==fen45) {
          menu=fen33;
        }
        else if (menu==fen46) {
          menu=fen34;
        }
        // Stop recording
        else if (menu==fen56) {
          ptr_journey->stop_recording();
          ptr_journey->save_on_EEPROM();
          menu=fen46;
        }
        break;
      
      // Button 2 is "up"
      case 2:
        // Preventing from jumping menus
        if (menu!=fen1&menu!=fen11&menu!=trip1&menu!=fen32&menu!=fen42&menu!=fen43&menu!=fen46&menu!=fen56) {
          menu=int(menu)-1;
        }
        //
        break;

      // Button 3 is "down"
      case 3:
        // Preventing from jumping menus
        if (menu!=fen2&menu!=fen12&menu!=trip10&menu!=fen34&menu!=fen42&menu!=fen45&menu!=fen46&menu!=fen56) {
          menu=int(menu)+1;
        }
        break;

      // Button 4 is "enter"
      case 4:
        // Preventing from jumping menus
        if (menu==fen11|menu==fen12|menu==fen42|menu==fen43|menu==fen44|menu==fen45|menu==fen56) {
          break;
        }
        // Enters "current coordinates" menu
        if (menu==fen1) {
          menu=fen11;
        }
        // Enters "memory slots" menu
        else if (menu==fen2) {
          menu=trip1;
        }
        // List of memory slots
        else if (menu==trip1|menu==trip2|menu==trip3|menu==trip4|menu==trip5|menu==trip6|menu==trip7|menu==trip8|menu==trip9|menu==trip10) {
          // Saving memory slot chosen by user
          currentTrip=menu;
          // Create a journey with currentTrip ID
          ptr_journey = new Journey(currentTrip);
          menu=fen32;
        }
        // Export to USB
        else if (menu==fen32) {
          menu=fen42;
        }
        // Display chosen trip datas'
        else if (menu==fen33) {
          menu=fen43;
        }
        // New trip confirmation
        else if (menu==fen34) {
          menu=fen46;
        }
        // Acquisition validation
        else if (menu==fen46) {
          ptr_journey->start_recording();
          menu=fen56;
        }
        break;
      }
    }

    switch(menu)
  {
    case fen1:
      lcd.setCursor(0,0);
      lcd.println("DISPLAY ");
      lcd.setCursor(0,1);
      lcd.print("COORD");
      //setCursor(1,2);
      //lcd.print("fen1");
      break;
    case fen11:
      lcd.setCursor(0,0);
      // X coordinates
      lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      lcd.setCursor(0,1);
      // Y coordinates
      lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      //setCursor(1,2);
      //lcd.print("fen11");
      break;
    case fen12:
      lcd.setCursor(0,0);
      lcd.print("Z");
      lcd.setCursor(0,1);
      lcd.print(falt);
      //setCursor(1,2);
      //lcd.print("fen12");
      break;
    case fen2:
      lcd.setCursor(0,0);
      lcd.print("RECORDS");
      //setCursor(1,2);
      //lcd.print("fen2");
      break;
    case trip1:
      lcd.setCursor(0,0);
      lcd.print("TRIP 1");
      //setCursor(1,2);
      //lcd.print("trip1");
      break;
    case trip2:
      lcd.setCursor(0,0);
      lcd.print("TRIP 2");
      //lcd.setCursor(1,2);
      //lcd.print("trip2");
      break;
    case trip3:
      lcd.setCursor(0,0);
      lcd.print("TRIP 3");
      //lcd.setCursor(1,2);
      //lcd.print("trip3");
      break;
    case trip4:
      lcd.setCursor(0,0);
      lcd.print("TRIP 4");
      //lcd.setCursor(1,2);
      //lcd.print("trip4");
      break;
    case trip5:
      lcd.setCursor(0,0);
      lcd.print("TRIP 5");
      //lcd.setCursor(1,2);
      //lcd.print("trip5");
      break;
    case trip6:
      lcd.setCursor(0,0);
      lcd.print("TRIP 6");
      //lcd.setCursor(1,2);
      //lcd.print("trip6");
      break;      
    case trip7:
      lcd.setCursor(0,0);
      lcd.print("TRIP 7");
      //lcd.setCursor(1,2);
      //lcd.print("trip7");
      break;
    case trip8:
      lcd.setCursor(0,0);
      lcd.print("TRIP 8");
      //lcd.setCursor(1,2);
      //lcd.print("trip8");
      break;
    case trip9:
      lcd.setCursor(0,0);
      lcd.print("TRIP 9");
      //lcd.setCursor(1,2);
      //lcd.print("trip9");
      break;    
    case trip10:
      lcd.setCursor(0,0);
      lcd.print("TRIP 10");
      //lcd.setCursor(1,2);
      //lcd.print("trip10");
      break;
    case fen32:
      lcd.setCursor(0,0);
      lcd.print("EXPORT");
      lcd.setCursor(0,1);
      lcd.print("TO USB");
      //lcd.setCursor(1,2);
      //lcd.print("fen32");
      break;
    case fen33:
      lcd.setCursor(0,0);
      lcd.print("DISPLAY");
      lcd.setCursor(0,1);
      lcd.print(" DATA ");
      //lcd.setCursor(1,2);
      //lcd.print("fen33");
      break;
    case fen42:
      lcd.setCursor(0,0);
      lcd.print("EXPORT");
      lcd.setCursor(0,1);
      lcd.print("IN PROG");
      
      // Exporting datas
      ptr_journey->print_coords();
      // When finished
      lcd.setCursor(0,1);
      lcd.print(" DONE ");
      menu = fen32;
      //lcd.setCursor(1,2);
      //lcd.print("fen42");
      break;
    case fen43:
      lcd.setCursor(0,0);
      lcd.print("SPEEDAVG");
      lcd.setCursor(0,1);
      lcd.print(ptr_journey->get_m_speed());
      //lcd.setCursor(1,2);
      //lcd.print("fen43");
      break;
    case fen44:
      lcd.setCursor(0,0);
      lcd.print("LENGTH");
      lcd.setCursor(0,1);
      lcd.println(ptr_journey->get_total_distance()/1000);
      //lcd.setCursor(1,2);
      //lcd.print("fen44");
      break;
    case fen45:
      lcd.setCursor(0,0);
      lcd.print("DURATION");
      lcd.setCursor(0,1);
      lcd.println(ptr_journey->get_time()/1000);
      //lcd.setCursor(1,2);
      //lcd.print("fen45");
      break;
    case fen34:
      lcd.setCursor(0,0);
      lcd.print("  NEW  ");
      lcd.setCursor(0,1);
      lcd.print(" ROUTE ");
      //lcd.setCursor(1,2);
      //lcd.print("fen34");
      break;
    case fen46:
      lcd.setCursor(0,0);
      lcd.print("BEGIN ?");
      //lcd.setCursor(1,2);
      //lcd.print("fen46");
      break;
    case fen56:
      lcd.setCursor(0,0);
      lcd.print("FETCHING");
      lcd.setCursor(0,1);
      lcd.print(" DATA ");
      //lcd.setCursor(1,2);
      //lcd.print("fen56");
      break;
    default:
      lcd.setCursor(1,2);
      lcd.print("REBOOT");
      break;
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
