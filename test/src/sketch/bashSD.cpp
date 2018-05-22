//#include <SD.h>
#include "bashSD.h"

// fonction which execute the command arguments and return a string result of command
/*String bashSD(String arguments){
    // do nothing
}*/

/*
    Definition for global
*/
bool newDataBash = false;
char receivedCharsBash[NUMCHARS];

void recvWithEndMarker() {
    static byte ndx = 0;
    char rc;
    
    // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newDataBash == false) {
        rc = Serial.read();

        if (rc != ENDMARKER) {
            receivedCharsBash[ndx] = rc;
            ndx++;
            if (ndx >= NUMCHARS) {
                ndx = NUMCHARS - 1;
            }
        }
        else {
            receivedCharsBash[ndx] = '\0'; // terminate the string
            ndx = 0;
            newDataBash = true;
        }
    }
}

void showNewData() {
    if (newDataBash == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedCharsBash);
    newDataBash = false;
    }
}


