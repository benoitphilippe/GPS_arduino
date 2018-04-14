#include "screen.h"

/**
 * Implementation of class screen
 */
 Screen::Screen():LiquidCrystal(RS, EN, D4, D5, D6, D7){}
 
 Screen::Screen(int8_t rs, int8_t en, int8_t d4, int8_t d5, int8_t d6, int8_t d7)
 :LiquidCrystal(rs, en, d4, d5, d6, d7){}
