#include "journey.h"
#include <EEPROM.h>
#include <SD.h>


/**
 * All principle journey informations are stored in EEPROM.
 * Because of memory limitations, only a few journey can be stored,
 * The maximun number of journey is defined in MAX_JOURNEY_IN_MEMORY constant.
 * 
 * In EEPROM, a journey is defined by these value :
 * - ID : a byte which specifies the ID of journey, can be between 1 and MAX_JOURNEY_IN_MEMORY.
 * - Time : unsigned long int : specifies the duration in milliseconds of journey, starting time is stored in SD memory
 * - Distance : a float which represents the total distance of the journey
 * 
 * Other informations are stored in SD memory, accessible by the ID value
 * (except Speed which is just Distance/Time)
 */ 

// Constructors
Journey::Journey(){
    // this constructor create a new journey in the first free memory space.
    // if no free memory is left, just overwrite on the last memory space.
    eeDatas.ID = 255;
    for (byte i = 0; i < MAX_JOURNEY_IN_MEMORY ; i++){
        if (!Journey::is_memory_place_occupied(i)){
            eeDatas.ID = i;
            break;
        }
    }
    if (eeDatas.ID == 255) eeDatas.ID = MAX_JOURNEY_IN_MEMORY - 1;
    eeDatas._total_distance = 0.0f;
    eeDatas._time = 0;
    // ready to be saved
}

Journey::Journey(const byte ID){
    // take a precise place of memory
    // if ID is greater than max journey allowded, just take the last ID allowded
    if (ID >=  MAX_JOURNEY_IN_MEMORY) eeDatas.ID = MAX_JOURNEY_IN_MEMORY -1;
    else eeDatas.ID = ID;
    // get data from existing EEPROM
    if (Journey::is_memory_place_occupied(eeDatas.ID)){
        EEPROM.get(eeDatas.ID * sizeof(EEPROM_datas), eeDatas);
    }
    else{ // else, start from zeros
        eeDatas._total_distance = 0.0f;
        eeDatas._time = 0;
    }
}

// methods that write on arduino system
void Journey::erase_from_memory(){
    for (int eeAddress = eeDatas.ID * sizeof(eeDatas); eeAddress < (eeDatas.ID + 1) * sizeof(eeDatas); eeAddress++){
        EEPROM.write(eeAddress, 255);
    }
    // TODO : erase data also from SD
}
void Journey::erase_all_points(){} // delete all the content in the SD file
void Journey::append_point(){} // save the current point on SD : lat, lon, date, time. from global val
void Journey::save_on_EEPROM(){
    // save self on EEPROM. /!\ EEPROM has limited usage. Use this method with precaution
    EEPROM.put(eeDatas.ID * sizeof(EEPROM_datas), eeDatas);
} 

// private static methods
bool Journey::is_memory_place_occupied(const byte ID){
    // read from EEPROM and return true if memory space is occupied
    if (ID >= MAX_JOURNEY_IN_MEMORY) return false; // ID superior to MAX_JOURNEY_IN_MEMORY are not occupied
    // else get answer from EEPROM
    int eeAddress = ID * sizeof(EEPROM_datas);
    if (EEPROM.read(eeAddress) == ID) return true;
    else return false;
}

// public static methods
void Journey::print_all_EEPROM(){
    // prints value of all journey in EEPROM: 
    //  Add ID  Time        Distance
    //  ----------------------------------------------
    //  0   0   2553341     25.310              valid journey
    //  9   255 FFFFFFFF    FFFFFFFF            none journey journey stored at this Address

    byte inc = 0;
    Serial.print("Add");
    Serial.print("\t");
    Serial.print("ID");
    Serial.print("\t");
    Serial.print("Time");
    Serial.print("\t");
    Serial.print("\t");
    Serial.print("Distance");
    Serial.println(" ");
    Serial.println("-------------------------------------------");
    for (
            int eeAdress = 0;
            eeAdress < EEPROM.length() && eeAdress < MAX_JOURNEY_IN_MEMORY * sizeof(EEPROM_datas);
            eeAdress += sizeof(EEPROM_datas)
    ){
        Serial.print(eeAdress);
        Serial.print("\t");
        if (Journey::is_memory_place_occupied(inc)){
            EEPROM_datas datas;
            EEPROM.get(eeAdress, datas);
            Serial.print(datas.ID);
            Serial.print("\t");
            Serial.print(datas._time);
            Serial.print("\t");
            Serial.print(datas._total_distance);
            Serial.println(" ");
        }
        else{
            Serial.print(EEPROM.read(eeAdress), DEC);
            Serial.print("\t");
            for (int i = eeAdress + sizeof(byte); i < eeAdress + sizeof(byte) + sizeof(unsigned long int); i++)
                Serial.print(EEPROM.read(i), HEX);
            Serial.print("\t");
            for (
                int i = eeAdress + sizeof(byte) + sizeof(unsigned long int);
                i < eeAdress + sizeof(byte) + sizeof(unsigned long int) + sizeof(float);
                i++
                ){
                    Serial.print(EEPROM.read(i), HEX);
                }
            Serial.println(" ");
        }
        inc++;
    }
}