#include <LiquidCrystal.h>

#define MAX_BATTERY 774.0

LiquidCrystal lcd(4,5,6,7,8,9);

void setup() {
  Serial.begin(9600);
  lcd.begin(8,2);
  while(!Serial) {
    ;
  }
  lcd.print("Battery");
  lcd.setCursor(0,1);
  lcd.print((analogRead(A0) * 100.0)/MAX_BATTERY);
  lcd.print('%');
  Serial.println("Système en cours d'initialisation");
  Serial.print("Battery lvl : ");
  Serial.print((analogRead(A0) * 100.0)/MAX_BATTERY);
  Serial.println('%');
  delay(2000);
}

void loop() {
}
