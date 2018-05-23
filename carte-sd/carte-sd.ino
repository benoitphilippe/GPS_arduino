#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "GPS.h"

// MAX_BATTERY
#define MAX_BATTERY 774.0
// We are using a board which uses the pin 10/SS for the SD card (CS/SS pin)
#define SDPORT 10

Sd2Card card;
SdVolume volume;
SdFile root;

// lcd printer
LiquidCrystal lcd(4,5,6,7,8,9);

// GPS serial and device
TinyGPS gps;
SoftwareSerial ss(3, 2);

// GPS global datas, filled with getGPSData()
float flat, flon;
unsigned long age;

void setup() {
  unsigned long startTime;
  unsigned long currentTime;
  //Serial.begin(9600);
  lcd.begin(8,2);
  ss.begin(4800);
  /*while(!Serial) {
    ;
  }*/
  lcd.print("Battery");
  lcd.setCursor(0,1);
  lcd.print(((float)analogRead(A0) * 100.0)/MAX_BATTERY);
  lcd.print('%');
  startTime = millis();
  
  if (!card.init(SPI_HALF_SPEED, SDPORT)) {
    //Serial.println("SD isnotok");
    while (1);
  }

  // print the type of card
  /*Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }*/  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    //Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }
/*
  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;

  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  "); 
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");*/
  root.openRoot(volume);

  // list all files in the card with date and size
  //root.ls(LS_R | LS_DATE | LS_SIZE);

  // test pour savoir si on a dépassé les 3 secondes de chargement
  currentTime = millis();
  if (currentTime - startTime < 3000){
    delay(3000 - (currentTime - startTime));
  }
}

void loop(void) {
  if (getGPSData())
    { // when new datas are available
        lcd.setCursor(0,0);
        lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        lcd.setCursor(0,1);
        lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        /*Serial.print("LAT=");
        Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        Serial.print(" LON=");
        Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        Serial.print(" SAT=");
        Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        Serial.print(" PREC=");
        Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());*/
    }
}
