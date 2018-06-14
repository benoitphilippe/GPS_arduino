#include "GPS.h"

/**
 * Finaly, after experimentation, all the angle stuffs are useless.
 * It is more convenient to limit DISTANCE_ERROR_TOLERANCE to 2 metters
 * in order to detect absence of movement.
 * This code can evaluate with precision total disctance by local movement.
 */ 

bool getGPSData(){
    static unsigned long timeLastMesure = 0; // control time
    static float m_flat = 0.0f, m_flon = 0.0f; // medium position computes in acquisition time
    static float m_falt = 0.0f; // la moyenne de l'altitude
    static byte npoints = 0; // points number in acquisition
    float l_flat, l_flon; // in fonction latitude and longitude
    float l_falt;
    while (ss.available())
    {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)){ // Did a new valid sentence come in?
            if (millis() - timeLastMesure > ACQUISITION_TIME){ // over acquisition time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position
                l_falt = gps.f_altitude(); // get current altitude

                // update one last time medium position
                if (npoints == 0){ // first point case
                    m_flat = l_flat;
                    m_flon = l_flon;
                    m_falt = l_falt;
                }
                else{ // computes medium position
                    npoints++;
                    // medium position : m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                    m_falt = m_falt + (l_falt - m_falt)/((float)npoints);
                }
                
                gps.get_datetime(&date, &time, &age);// time in hhmmsscc, date in ddmmyy
                timeLastMesure = millis(); // update time mesure

                // get distance between two points
                if (flat != 0.0f && flon != 0.0f){
                    l_distance = TinyGPS::distance_between(m_flat, m_flon, flat, flon);
                    // check for validity of distance
                    if(l_distance < DISTANCE_ERROR_TOLERANCE){ // distance is low
                        l_distance = 0; // we suppose position is'nt moving
                    }
                }

                // update last medium position with new medium position
                flat = m_flat;
                flon = m_flon;
                falt = m_falt;
            
                npoints = 0; // reset medium computage
                return true; // notify next position is ready
            }
            else { // in acquisition time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position
                l_falt = gps.f_altitude();                  // get current altitude
                // add position in medium position, and statdard deviation
                if (npoints == 0){ // first point case
                    // initiate all datas
                    m_flat = l_flat;
                    m_flon = l_flon;
                    m_falt = l_falt;
                    npoints++;
                }
                else{ // computes medium position
                    npoints++;
                    // m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                    m_falt = m_falt + (l_falt - m_falt) / ((float)npoints);
                }
                // still in acquisition time, so no data to return yet
                return false;
            }
        }
    }
    // no valid sentence come in
    return false;
}