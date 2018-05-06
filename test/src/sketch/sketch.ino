#include "screen.h"
/**
 * Remider conf arduino
 * booard type : pro mini
 * microcontroler : ATmega328 8bits 8Mhz
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
