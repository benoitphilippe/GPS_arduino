#include "Bounce2.h"
#include <LiquidCrystal.h>

//Valeur batterie maximum
#define MAX_BATTERY 774.0

//Boutons
#define BP0 16 
#define BP1 15
#define BPEN 17

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
// if button is pushed 1, else 0
int current_enable;

int input = 0;
enum fenetres {
  trip1, trip2, trip3, trip4, trip5, trip6, trip7, trip8, trip9, trip10, fen1, fen2, fen11, fen12, fen32, fen33, fen34, fen42, fen43, fen44, fen45, fen46, fen56
  };
fenetres menu = fen1;
//pour gérer le trip à exporter/afficher les datas, à changer dans les fen22-31
int currentTrip; 
LiquidCrystal lcd(4,5,6,7,8,9);

void setup() {

  //DEBUG
  ///////////////////////
  
  lcd.begin(8,2);

  pinMode(BP0, INPUT_PULLUP);
  debouncer1.attach(BP0);
  debouncer1.interval(50);
  
  pinMode(BP1, INPUT_PULLUP);
  debouncer2.attach(BP1);
  debouncer2.interval(50);

  pinMode(BPEN, INPUT_PULLUP);
  debouncer3.attach(BPEN);
  debouncer3.interval(50);
  
  // Initilisation : afficher batterie
  lcd.print("BATTERY");
  lcd.setCursor(0,1);
  lcd.print(((float)analogRead(A0) * 100.0)/MAX_BATTERY);
  lcd.print('%');
  delay(3000);
  lcd.clear();
  delay(100);
}

void loop() {
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
        else if (menu==fen32|menu==fen33|menu==fen34) {
          menu=trip1;
        }
        // Stop data export
        else if (menu==fen42) {
          menu=fen32;
        }
        else if (menu==fen43|menu==fen44|menu==fen45) {
          menu=fen33;
        }
        else if (menu==fen46) {
          menu=fen34;
        }
        // Stop data acquistion
        else if (menu==fen56) {
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
          menu=fen32;
        }
        // Export to USB
        else if (menu==fen32) {
          //
          menu=fen42;
        }
        // Display chosen trip datas'
        else if (menu==fen33) {
          menu=fen43;
        }
        // New trip
        else if (menu==fen34) {
          menu=fen46;
        }
        // Acquisition validation
        else if (menu==fen46) {
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
      lcd.print("X");
      lcd.setCursor(0,1);
      lcd.print("Y");
      //setCursor(1,2);
      //lcd.print("fen11");
      break;
    case fen12:
      lcd.setCursor(0,0);
      lcd.print("Z");
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
      //lcd.setCursor(1,2);
      //lcd.print("fen42");
      break;
    case fen43:
      lcd.setCursor(0,0);
      lcd.print("SPEEDAVG");
      lcd.setCursor(0,1);
      lcd.print("SAVAVIT");
      //lcd.setCursor(1,2);
      //lcd.print("fen43");
      break;
    case fen44:
      lcd.setCursor(0,0);
      lcd.print("LENGTH");
      lcd.setCursor(0,1);
      lcd.print("CMB");
      //lcd.setCursor(1,2);
      //lcd.print("fen44");
      break;
    case fen45:
      lcd.setCursor(0,0);
      lcd.print("DURATION");
      lcd.setCursor(0,1);
      lcd.print("4EVER");
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
}
