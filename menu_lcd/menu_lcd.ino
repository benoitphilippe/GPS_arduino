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
fenetres menuTest = fen1;
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
  lcd.print(((int)menuTest));
  lcd.setCursor(0,1);
  lcd.print(((int)current_enable));
  lcd.setCursor(7,1);
  if (input != 0) {
    lcd.clear();
      
      switch(input)
    {
      case 1:
        if (menuTest==fen11|menuTest==fen12|menuTest==fen13|menuTest==fen14) {
          menuTest=fen1;
        }
        else if (menuTest==fen22) {
          menuTest=fen2;
        }
        else if (menuTest==fen32) {
          menuTest=fen22;
        }
        else if (menuTest==fen43|menuTest==fen44|menuTest==fen45|menuTest==fen46|menuTest==fen47|menuTest==fen48|menuTest==fen49|menuTest==fen50|menuTest==fen51|menuTest==fen52) {
          menuTest=fen3;
        }
        else if (menuTest==fen63|menuTest==fen64) {
          menuTest=fen43;
        }
        else if (menuTest==fen73) {
          menuTest=fen63;
        }
        else if (menuTest==fen74|menuTest==fen75|menuTest==fen76) {
          menuTest=fen64;
        }
        break;

      case 2:
        if (menuTest!=fen1&menuTest!=fen11&menuTest!=fen22&menuTest!=fen32&menuTest!=fen43&menuTest!=fen63&menuTest!=fen73&menuTest!=fen74) {
          menuTest=int(menuTest)-1;
        }
        //
        break;
        
      case 3:
        if (menuTest!=fen3&menuTest!=fen14&menuTest!=fen22&menuTest!=fen32&menuTest!=fen52&menuTest!=fen64&menuTest!=fen73&menuTest!=fen76) {
          menuTest=int(menuTest)+1;
        }
        break;

      case 4:
        if (menuTest==fen11|menuTest==fen12|menuTest==fen13|menuTest==fen14|menuTest==fen32|menuTest==fen73|menuTest==fen74|menuTest==fen75|menuTest==fen76) {
          break;
        }
        if (menuTest==fen1) {
          menuTest=fen11;
        }
        else if (menuTest==fen2) {
          menuTest=fen22;
        }
        else if (menuTest==fen22) {
          menuTest=fen32;
        }
        else if (menuTest==fen3) {
          menuTest=fen43;
        }
        else if (menuTest==fen43|menuTest==fen44|menuTest==fen45|menuTest==fen46|menuTest==fen47|menuTest==fen48|menuTest==fen49|menuTest==fen50|menuTest==fen51|menuTest==fen52) {
          menuTest=fen63;
        }
        else if (menuTest==fen63) {
          menuTest=fen73;
        }
        else if (menuTest==fen64) {
          menuTest=fen74;
        }
        break;
      }
    }

    switch(menuTest)
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
