#include "GPS.h"

bool getGPSData(){
    static unsigned long timeLastMesure = 0; // control time
    float l_flat, l_flon;
    float l_distance; // distance in kms
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){ // Did a new valid sentence come in?
            if (millis() - timeLastMesure > 1000){ // check for 1 seconde time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position
                
                gps.get_datetime(&date, &time, &age);// time in hhmmsscc, date in ddmmyy
                timeLastMesure = millis(); // update time mesure

                // get distance between two points
                if (flat != 0.0f && flon != 0.0f){
                    l_distance = TinyGPS::distance_between(l_flat, l_flon, flat, flon) / 1000.0f;
                    if(l_distance < DISTANCE_ERROR_TOLERANCE)
                        l_distance = 0;
                }
                total_distance += l_distance;
                // update last position with new position
                flat = l_flat;
                flon = l_flon;
                return true;
            }
        }
    }
    // no valid sentence come in or not the right time
    return false;
}

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = * ( long * ) &y; // evil floating point bit level hacking
	i = 0x5f3759df - ( i >> 1 ); // what the fuck?
	y = * ( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
//	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}