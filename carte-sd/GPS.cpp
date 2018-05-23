#include "GPS.h"

bool getGPSData(){
    //static unsigned long timeLastMesure = 0;
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){ // Did a new valid sentence come in?
            gps.f_get_position(&flat, &flon, &age); // get position
            return true;
        }

    }
    // no valid sentence come in or not the right time
    return false;
}