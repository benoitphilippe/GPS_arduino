#include "GPS.h"

/**
 * Finaly, after experimentation, all the angle stuffs are useless.
 * It is more convenient to limit DISTANCE_ERROR_TOLERANCE to 0.002 kms
 * in order to detect absence of movement.
 * This code can evaluate with precision total disctance by local movement.
 */ 

bool getGPSData(){
    static unsigned long timeLastMesure = 0; // control time
    static float m_flat = 0.0f, m_flon = 0.0f; // medium position computes in acquisition time
    static byte npoints = 0; // points number in acquisition
    float l_flat, l_flon; // in fonction latitude and longitude
    float l_distance; // distance in kms
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){ // Did a new valid sentence come in?
            if (millis() - timeLastMesure > ACQUISITION_TIME){ // over acquisition time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position

                // update one last time medium position
                if (npoints == 0){ // first point case
                    m_flat = l_flat;
                    m_flon = l_flon;
                }
                else{ // computes medium position
                    npoints++;
                    float last_m_flat = m_flat, last_m_flon = m_flon; // save last value
                    // medium position : m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                }
                
                gps.get_datetime(&date, &time, &age);// time in hhmmsscc, date in ddmmyy
                timeLastMesure = millis(); // update time mesure

                // get distance between two points
                if (flat != 0.0f && flon != 0.0f){
                    l_distance = TinyGPS::distance_between(m_flat, m_flon, flat, flon) / 1000.0f;
                    // check for validity of distance
                    if(l_distance < DISTANCE_ERROR_TOLERANCE){ // distance is low
                        l_distance = 0; // we suppose position is'nt moving
                    }
                }
                total_distance += l_distance; // total distance for this path
                // update last medium position with new medium position
                flat = m_flat;
                flon = m_flon;
            
                npoints = 0; // reset medium computage
                return true; // notify next position is ready
            }
            else { // in acquisition time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position
                // add position in medium position, and statdard deviation
                if (npoints == 0){ // first point case
                    // initiate all datas
                    m_flat = l_flat;
                    m_flon = l_flon;
                    npoints++;
                }
                else{ // computes medium position
                    npoints++;
                    float last_m_flat = m_flat, last_m_flon = m_flon;
                    // m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                }
                // still in acquisition time, so no data to return yet
                return false;
            }
        }
    }
    // no valid sentence come in
    return false;
}