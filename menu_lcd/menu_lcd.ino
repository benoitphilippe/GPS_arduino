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

int action = 0;
int choix = 0;

LiquidCrystal lcd(4,5,6,7,8,9);

void setup() {
  
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
  action = choix();

}
