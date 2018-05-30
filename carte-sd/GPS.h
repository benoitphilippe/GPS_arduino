/**
    File wich define how the board get the GPS Data
    all fonctions are based on the global vars TinyGPS gps
    and SoftwareSerial ss(3, 2)
*/
#ifndef GPS_H
#define GPS_H

// includes
#include <SoftwareSerial.h>
#include "TinyGPS.h"

/**
    Get some extern global vars
*/
extern SoftwareSerial ss;
extern float flat, flon;
extern unsigned long age;
extern TinyGPS gps;

/**
    This fonction fill and update all GPS datas
    return True when new data is available
*/
bool getGPSData();

#endif
