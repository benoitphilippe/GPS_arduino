#include "screen.h"
#include "bashSD.h"
/**
 * Remider conf arduino
 * booard type : pro mini
 * microcontroler : ATmega328 8bits 8Mhz
 */

void setup()
{
  // define baud rate for communication
  Serial.begin(BAUD_RATE);
  // initiate lcd screen
  lcd.begin(WIDTH_SCREEN, HEIGHT_SCREEN);
  // print simple message on lcd screen
  // lcd.print("Bonjour");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println(">> ");
}

void loop()
{
  recvWithEndMarker();
  showNewData();
  /*lcd.setCursor(0,1);
  lcd.print(millis() / 1000);
  // Serial.println(millis() / 1000);
  */
}
