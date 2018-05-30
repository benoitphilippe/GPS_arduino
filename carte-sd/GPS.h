/**
    File wich define how the board get the GPS Data
    all fonctions are based on the global vars TinyGPS gps
    and SoftwareSerial ss(3, 2)
*/
#ifndef GPS_H
#define GPS_H

// defines
#define EARTH_RAYON 6371000.0 // earth rayon in metters
#define DISTANCE_ERROR_TOLERANCE 0.002f // in kms
#define ACQUISITION_TIME 2000 // acquisition time in milliseconds
#define PI 3.1415927410125732421875f
// #define COURSE_DEVIATION_TOLERANCE 90.0f

// includes
#include <SoftwareSerial.h>
#include "TinyGPS.h"

/**
    Get some extern global vars
*/
extern SoftwareSerial ss;
extern float flat, flon;
extern unsigned long age;
extern unsigned long time, date;
extern TinyGPS gps;
extern float total_distance;
extern bool isRecording;

/**
    This fonction fill and update all GPS datas
    return True when new data is available
*/
bool getGPSData();

#endif
