#include "screen.h"
/**
 * Remider conf arduino
 * booard type : pro mini
 * microcontroler : ATmega328 8bits 8Mhz
 */

Screen lcd;
//LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/**
 * Memory use
 * Screen lcd -> 61 o
 * Screen lcd(...) -> 62 o
 * LiquidCrystal(...) -> 49 o
 */

void setup()
{
  lcd.begin(WIDTH_SCREEN, HEIGHT_SCREEN);
  lcd.print("LOL");
}

void loop()
{
  lcd.setCursor(0,1);
  lcd.print(millis() / 1000);
}
