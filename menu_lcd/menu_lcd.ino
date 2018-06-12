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
int current_enable;

int action = 0;
int input = 0;
enum fenetres {
  fen1, fen2, fen3, fen11, fen12, fen13, fen14, fen22, fen32, fen43, fen44, fen45, fen46, fen47, fen48, fen49, fen50, fen51, fen52, fen63, fen64, fen73, fen74, fen75, fen76
  };
fenetres menu = fen1;
//currentTrip pour gérer le trip à exporter/afficher les datas, à changer dans les fen43-52

LiquidCrystal lcd(4,5,6,7,8,9);

void setup() {

  //DEBUG
  ///////////////////////
  
  lcd.begin(8,2);

  pinMode(BP0, INPUT_PULLUP);
  debouncer1.attach(BP0);
  debouncer1.interval(5);
  
  pinMode(BP1, INPUT_PULLUP);
  debouncer2.attach(BP1);
  debouncer2.interval(5);

  pinMode(BPEN, INPUT_PULLUP);
  debouncer3.attach(BPEN);
  debouncer3.interval(5);
  
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
  lcd.setCursor(1,0);
  lcd.print(((int)menu));
  lcd.setCursor(0,1);
  lcd.print(((int)current_enable));
  lcd.setCursor(7,1);
  if (input != 0) {
    lcd.clear();
      
      switch(input)
    {
      case 1:
        if (menu==fen11|menu==fen12|menu==fen13|menu==fen14) {
          menu=fen1;
        }
        else if (menu==fen22) {
          menu=fen2;
        }
        else if (menu==fen32) {
          menu=fen22;
        }
        else if (menu==fen43|menu==fen44|menu==fen45|menu==fen46|menu==fen47|menu==fen48|menu==fen49|menu==fen50|menu==fen51|menu==fen52) {
          menu=fen3;
        }
        else if (menu==fen63|menu==fen64) {
          menu=fen43;
        }
        else if (menu==fen73) {
          menu=fen63;
        }
        else if (menu==fen74|menu==fen75|menu==fen76) {
          menu=fen64;
        }
        break;

      case 2:
        if (menu!=fen1&menu!=fen11&menu!=fen22&menu!=fen32&menu!=fen43&menu!=fen63&menu!=fen73&menu!=fen74) {
          menu=int(menu)-1;
        }
        //
        break;
        
      case 3:
        if (menu!=fen3&menu!=fen14&menu!=fen22&menu!=fen32&menu!=fen52&menu!=fen64&menu!=fen73&menu!=fen76) {
          menu=int(menu)+1;
        }
        break;

      case 4:
        if (menu==fen11|menu==fen12|menu==fen13|menu==fen14|menu==fen32|menu==fen73|menu==fen74|menu==fen75|menu==fen76) {
          break;
        }
        if (menu==fen1) {
          menu=fen11;
        }
        else if (menu==fen2) {
          menu=fen22;
        }
        else if (menu==fen22) {
          menu=fen32;
        }
        else if (menu==fen3) {
          menu=fen43;
        }
        else if (menu==fen43|menu==fen44|menu==fen45|menu==fen46|menu==fen47|menu==fen48|menu==fen49|menu==fen50|menu==fen51|menu==fen52) {
          menu=fen63;
        }
        else if (menu==fen63) {
          menu=fen73;
        }
        else if (menu==fen64) {
          menu=fen74;
        }
        break;
      }
    }

    switch(menu)
  {
    case fen1:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DISPLAY");
      lcd.setCursor(1,2);
      lcd.print("fen1");
      break;
    case fen11:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DISPLAY");
      lcd.setCursor(1,2);
      lcd.print("fen11");
      break;
    case fen12:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DISPLAY");
      lcd.setCursor(1,2);
      lcd.print("fen12");
      break;
    case fen13:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DISPLAY");
      lcd.setCursor(1,2);
      lcd.print("fen13");
      break;
    case fen14:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DISPLAY");
      lcd.setCursor(1,2);
      lcd.print("fen14");
      break;
    case fen2:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("NEW");
      lcd.setCursor(1,2);
      lcd.print("fen2");
      break;
    case fen22:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("NEW");
      lcd.setCursor(1,2);
      lcd.print("fen22");
      break;
    case fen32:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("NEW");
      lcd.setCursor(1,2);
      lcd.print("fen32");
      break;
    case fen3:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen3");
      break;
    case fen43:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen43");
      break;
    case fen44:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen44");
      break;
    case fen45:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen45");
      break;
    case fen46:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen46");
      break;
    case fen47:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen47");
      break;
    case fen48:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen48");
      break;
    case fen49:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen49");
      break;
    case fen50:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen50");
      break;
    case fen51:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen51");
      break;
    case fen52:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen52");
      break;
    case fen63:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen63");
      break;
    case fen64:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen64");
      break;
    case fen73:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen73");
      break;
    case fen74:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen74");
      break;
    case fen75:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen75");
      break;
    case fen76:
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("PREVIOUS");
      lcd.setCursor(1,2);
      lcd.print("fen76");
      break;
    default:
      //lcd.clear();
      lcd.setCursor(1,2);
      lcd.print("DEF");
      break;
  }
}
