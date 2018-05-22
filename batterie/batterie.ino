#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

Sd2Card card;
SdVolume volume;
SdFile root;

LiquidCrystal lcd(4,5,6,7,8,9);

// We are using a board which uses the pin 10/SS for the SD card (CS/SS pin)
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  lcd.begin(8,2);
  while(!Serial) {
    ;
  }
  Serial.println("\nInitializing SD card...");
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed, things to check:");
    Serial.println("* is a card inserted ?");
    Serial.println("* is your wiring correct ?");
    Serial.println("* did you change the chipSelect pin to match your shield or module ?");
    Serial.print("isnotok");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
    Serial.println("is ok");
  }
}

void loop() {
  lcd.print("is ok");

}
