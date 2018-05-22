#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"

#define MAX_BATTERY 774.0

LiquidCrystal lcd(4,5,6,7,8,9);
TinyGPS gps;
SoftwareSerial ss(3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(8,2);
  ss.begin(4800);
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

  Serial.println("");
  Serial.print("Simple TinyGPS library v. "); 
  Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
}

void loop() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
