#include "GPS.h"

bool getGPSData(){
    static unsigned long timeLastMesure = 0; // control time
    static float m_flat = 0.0f, m_flon = 0.0f; // medium position computes in acquisition time
    static float standard_deviation_flat = 0.0f, standard_deviation_flon = 0.0f;
    static byte npoints = 0; // points number in acquisition
    static float m_course = 0.0f; // medium angle progression
    static float standard_deviation_course = 0.0f;
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
                    standard_deviation_flat = 0.0f;
                    standard_deviation_flon = 0.0f;
                    standard_deviation_course = 0.0f;
                    m_course = gps.f_course();
                }
                else{ // computes medium position
                    npoints++;
                    float last_m_flat = m_flat, last_m_flon = m_flon; // save last value
                    float last_m_course = m_course;
                    // medium position : m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                    // standard derivation
                    // Nk = N(k-1) + (Xk - m_Xk)(Xk - m_X(k-1))
                    standard_deviation_flat += (l_flat - m_flat)*(l_flat - last_m_flat);
                    standard_deviation_flon += (l_flon - m_flon)*(l_flon - last_m_flon);
                    // compute medium angle mouvement
                    m_course += (gps.f_course() - m_course)/((float)npoints);
                    // compute standard deviation course
                    standard_deviation_course += (gps.f_course() - m_course)*(gps.f_course() - last_m_course);

                }
                
                gps.get_datetime(&date, &time, &age);// time in hhmmsscc, date in ddmmyy
                timeLastMesure = millis(); // update time mesure
                

                // get distance between two points
                if (flat != 0.0f && flon != 0.0f){
                    l_distance = TinyGPS::distance_between(m_flat, m_flon, flat, flon) / 1000.0f;
                    // check for validity of distance
                    if(l_distance < DISTANCE_ERROR_TOLERANCE){ // distance is low
                        // look for angle deviation
                        if (standard_deviation_course > COURSE_DEVIATION_TOLERANCE){ // angle deviation is high
                            l_distance = 0; // we suppose position is'nt moving
                        } 
                        else if (abs((int)(TinyGPS::course_to(m_flat, m_flon, flat, flon))) > COURSE_DEVIATION_TOLERANCE){
                            // Angle difference between the last two validated points is high
                            l_distance = 0; 
                        }
                    }
                }
                total_distance += l_distance; // total distance for this path
                // update last medium position with new medium position
                flat = m_flat;
                flon = m_flon;
                // notify next position is ready
                Serial.print("distance = ");
                Serial.println(total_distance, 3);
                Serial.print("number of points : ");
                Serial.println(npoints);
                Serial.print("Ecart-type");
                Serial.print(standard_deviation_flat);
                Serial.print(", ");
                Serial.println(standard_deviation_flon);
                Serial.print("angle : ");
                Serial.println(m_course);
                Serial.print("Ecart-type");
                Serial.println(standard_deviation_course);
                npoints = 0; // reset medium computage
                return true;
            }
            else { // in acquisition time
                gps.f_get_position(&l_flat, &l_flon, &age); // get current position
                // add position in medium position, and statdard deviation
                if (npoints == 0){ // first point case
                    // initiate all datas
                    m_flat = l_flat;
                    m_flon = l_flon;
                    npoints++;
                    standard_deviation_flat = 0.0f;
                    standard_deviation_flon = 0.0f;
                    m_course = gps.f_course();
                    standard_deviation_course = 0.0f;
                }
                else{ // computes medium position
                    npoints++;
                    float last_m_flat = m_flat, last_m_flon = m_flon;
                    float last_m_course = m_course;
                    // m_Xk = m_X(k-1) + (Xk - m_X(k-1))/k
                    m_flat = m_flat + (l_flat - m_flat)/((float)npoints);
                    m_flon = m_flon + (l_flon - m_flon)/((float)npoints);
                    // Nk = N(k-1) + (Xk - m_Xk)(Xk - m_X(k-1))
                    standard_deviation_flat += (l_flat - m_flat)*(l_flat - last_m_flat);
                    standard_deviation_flon += (l_flon - m_flon)*(l_flon - last_m_flon);
                    // compute medium angle mouvement
                    m_course += (gps.f_course() - m_course)/((float)npoints);
                    // compute standard deviation course
                    standard_deviation_course += (gps.f_course() - m_course)*(gps.f_course() - last_m_course);
                }
                // still in acquisition time, so no data to return yet
                return false;
            }
        }
    }
    // no valid sentence come in
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