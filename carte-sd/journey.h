#ifndef JOURNEY_H
#define JOURNEY_H

//include 
#include <EEPROM.h>
#include <SD.h>

//defines
#define MAX_JOURNEY_IN_MEMORY 10

//extern value
extern float flat, flon;
extern float total_distance;
extern unsigned long age;
extern unsigned long time, date;
extern unsigned long startTime, JourneyTime;

// private Structure
struct EEPROM_datas{
    byte ID; // ID of journey
    float _total_distance; // total distance in kms
    unsigned long int _time; // total time duration of journey in milliseconds

};

// class definition
class Journey{
    public:
        // Contructors
        Journey(); // create a new Journey in free space of memory
        Journey(const byte ID); // take an existing journey, or take this special space of memory

        // methods that read from memory
        void print_coords(); // print on serial all the coords from this Journey (from SD)
        void print_EEPROM(); // print on serial ID, distance, time and speed

        // methods that write on arduino system
        void erase_from_memory(); // erase the current Journey from SD and EEPROM
        void erase_all_points(); // delete all the content in the SD file
        void append_point(); // save the current point on SD : lat, lon, date, time. from global val
        void save_on_EEPROM(); // save self on EEPROM. /!\ EEPROM has limited usage. Use this method with precaution
        
        // public static methods
        static void print_all_EEPROM();
        // specialised getters
        float get_m_speed(); // get medium speed in km/h 
        unsigned long get_start_date(); // return the start date of journey
        unsigned long get_start_time(); // return the start time of journey
        
        // getters for privates attributes
        byte get_ID() const;
        float get_total_distance() const;
        unsigned long int get_time() const;
        bool is_recording() const;

    private:
        // attributes
        bool _is_recording; // is journey is currently recording
        EEPROM_datas eeDatas; // datas in EEPROM
        
        // private static method
        static bool is_memory_place_occupied(const byte ID);

};

// start recording new journey
void newJourney();
// end recording current journey
void endJourney();

#endif