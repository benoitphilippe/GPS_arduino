#ifndef SCREEN_H
#define SCREEN_H

#include <LiquidCrystal.h>

// screen size

#define WIDTH_SCREEN 8
#define HEIGHT_SCREEN 2
#define BAUD_RATE 4800

// defines for screen interface
#define RS 4
#define EN 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

// define lcd
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

#endif
