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
 * - Time : unsigned long int : specifies the duration in milltime_last_record = millis();iseconds of journey, starting time is stored in SD memory
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

// methods that read from memory
void Journey::print_coords(){ // print on serial all the coords from this Journey (from SD)
    if (is_recording()) return; // can't read file if recording
    String filename = "";
    String suffix = ".txt";
    filename = filename + eeDatas.ID + suffix;
    if (file) file.close();
    file = SD.open(filename , FILE_READ);
    if (file) { // read the file
        while(file.available()){
            Serial.print(file.read());
        }
        file.close();
    }
    else return;
}
void Journey::print_EEPROM(){ // print on serial ID, distance, time and speed
    if (is_recording()) return; // can't read EEPROM if recording
    int eeAddress = eeDatas.ID * sizeof(EEPROM_datas);
    Serial.print(EEPROM.read(eeAddress), DEC);
    Serial.print("\t");
    for (int i = eeAddress + sizeof(byte); i < eeAddress + sizeof(byte) + sizeof(unsigned long int); i++)
        Serial.print(EEPROM.read(i), HEX);
    Serial.print("\t");
    for (
        int i = eeAddress + sizeof(byte) + sizeof(unsigned long int);
        i < eeAddress + sizeof(byte) + sizeof(unsigned long int) + sizeof(float);
        i++
        ){
            Serial.print(EEPROM.read(i), HEX);
        }
    Serial.println(" ");
}

// specialised getters
float Journey::get_m_speed(){ // get medium speed in km/h 
    float speed_m_per_s = eeDatas._total_distance / float(eeDatas._time / 1000);
    return 3.6f * speed_m_per_s;
}
unsigned long Journey::get_start_date() // return the start date of journey
{
    if (is_recording()) return 0; // can't read file if recording
    String filename = "";
    String suffix = ".txt";
    filename = filename + eeDatas.ID + suffix;
    if (file) file.close();
    file = SD.open(filename , FILE_READ);
    if (file) {
        char data_content[7];
        byte index = 0;
        byte nb_separator = 0;
        while (file.available())
        {
            char aChar = file.read();
            if(aChar == SEPARATOR){ // count separator
                nb_separator++;
            }
            else if(nb_separator == 2){ // data we want
                data_content[index++] = aChar; // fill data content
                data_content[index] = '\0';
            }
            else if (nb_separator > 2){
                if(strlen(data_content) > 0)
                {
                    unsigned long date = atol(data_content);
                    file.close();
                    return date;
                }
                else { // error occured
                    file.close();
                    return 0;
                }
            }
            else if(aChar == '\n' || aChar == '\r')// the character is CR or LF
            {
                break;
            }
        }
        file.close();
        return 0;
    }
    else return 0;
}
unsigned long Journey::get_start_time() // return the start time of journey
{
    if (is_recording()) return 0; // can't read file if recording
    String filename = "";
    String suffix = ".txt";
    filename = filename + eeDatas.ID + suffix;
    if (file) file.close();
    file = SD.open(filename , FILE_READ);
    if (file) {
        char data_content[9];
        byte index = 0;
        byte nb_separator = 0;
        while (file.available())
        {
            char aChar = file.read();
            if(aChar == SEPARATOR){ // count separator
                nb_separator++;
            }
            else if(nb_separator == 3){ // data we want
                data_content[index++] = aChar; // fill data content
                data_content[index] = '\0';
            }
            else if (aChar == '\n' || aChar == '\r'){
                if(strlen(data_content) > 0)
                {
                    unsigned long time = atol(data_content);
                    file.close();
                    return time;
                }
                else { // error occured
                    file.close();
                    return 0;
                }
            }
        }
        file.close();
        return 0;
    }
    else return 0;
}

// specialised setters
void Journey::start_recording(){ // start recording journey
    _is_recording = true;
    String filename = "";
    String suffix = ".txt";
    filename = filename + eeDatas.ID + suffix;
    if(file){
        file.close();
    }
    file = SD.open(filename , FILE_WRITE);
    time_last_record = millis();
}
void Journey::end_recording(){ // end recording current journey
    _is_recording = false;
    if(file){
        file.close();
    }
} 
void Journey::update_datas(){ // update distance and time
    if (!is_recording()) return;
    eeDatas._total_distance += l_distance;
    eeDatas._time += (millis() - time_last_record);
    time_last_record = millis();
}
// methods that write on arduino system
void Journey::erase_from_memory(){
    if(is_recording()) end_recording();
    for (int eeAddress = eeDatas.ID * sizeof(eeDatas); eeAddress < (eeDatas.ID + 1) * sizeof(eeDatas); eeAddress++){
        EEPROM.write(eeAddress, 255);
    }
    erase_all_points();
}
void Journey::erase_all_points(){ // delete all the content in the SD file
    if (is_recording() == true) return; // cannot delete file if still recording
    String filename = "";
    String suffix = ".txt";
    filename = filename + eeDatas.ID + suffix;
    if (SD.exists(filename)){
        SD.remove(filename);
    }
} 
void Journey::append_point(){ // save the current point on SD : lat, lon, date, time. from global val
    if (is_recording() == false) return; // do nothing if not recording
    // journey is recording, get acces to SD by attribute file
    if (file){ // ready to write
        // write datas
        file.print(flat);
        file.print(SEPARATOR);
        file.print(flon);
        file.print(SEPARATOR);
        file.print(date);
        file.print(SEPARATOR);
        file.println(time);
        file.flush(); // flush the stream
    }
}
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
    //  9   255 FFFFFFFF    FFFFFFFF            none journey stored at this Address

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
            int eeAddress = 0;
            eeAddress < EEPROM.length() && eeAddress < MAX_JOURNEY_IN_MEMORY * sizeof(EEPROM_datas);
            eeAddress += sizeof(EEPROM_datas)
    ){
        Serial.print(eeAddress);
        Serial.print("\t");
        if (Journey::is_memory_place_occupied(inc)){
            EEPROM_datas datas;
            EEPROM.get(eeAddress, datas);
            Serial.print(datas.ID);
            Serial.print("\t");
            Serial.print(datas._time);
            Serial.print("\t");
            Serial.print(datas._total_distance);
            Serial.println(" ");
        }
        else{
            Serial.print(EEPROM.read(eeAddress), DEC);
            Serial.print("\t");
            for (int i = eeAddress + sizeof(byte); i < eeAddress + sizeof(byte) + sizeof(unsigned long int); i++)
                Serial.print(EEPROM.read(i), HEX);
            Serial.print("\t");
            for (
                int i = eeAddress + sizeof(byte) + sizeof(unsigned long int);
                i < eeAddress + sizeof(byte) + sizeof(unsigned long int) + sizeof(float);
                i++
                ){
                    Serial.print(EEPROM.read(i), HEX);
                }
            Serial.println(" ");
        }
        inc++;
    }
}

// getters for privates attributes
byte Journey::get_ID() const {return eeDatas.ID;}
float Journey::get_total_distance() const {return eeDatas._total_distance;}
unsigned long int Journey::get_time() const {return eeDatas._time;}
bool Journey::is_recording() const {return _is_recording;}