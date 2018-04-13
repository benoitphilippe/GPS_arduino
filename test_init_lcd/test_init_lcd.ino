#include <LiquidCrystal.h>

LiquidCrystal lcd(4,5,6,7,8,9);

void setup() {
  lcd.begin(8,2);
  lcd.print("hello");
  lcd.setCursor(0,1);
  lcd.print("world!");
}

void loop() {
  lcd.setCursor(7,1);
  lcd.print(millis()/1000);
}
