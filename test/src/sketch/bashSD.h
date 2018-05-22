#ifndef BASHSD_H
#define BASHSD_H

#include <Arduino.h>

#define NUMCHARS 32
#define NUMFIELD 2
#define WORDMARKER ' '
#define ENDMARKER '\n'

// Définition of bashsd, take a string as argument, execute the function, return the string response
//String bashSD(String arguments);
void recvWithEndMarker();
void showNewData();

#endif
