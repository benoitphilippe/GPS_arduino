#ifndef SCREEN_H
#define SCREEN_H

#include <LiquidCrystal.h>

/**
 * Déclaration of screen class
 */

// screen size

#define WIDTH_SCREEN 8
#define HEIGHT_SCREEN 2

// defines for screen interface
#define RS 4
#define EN 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

/**
 * Screen classe inherits from LiquiCrystal
 * by this way, we can add some specific methods
 */
class Screen : public LiquidCrystal{
  public:
    /**
     * Constructors 
     */
    Screen();
    Screen(int8_t rs, int8_t en, int8_t d4, int8_t d5, int8_t d6, int8_t d7);
};

#endif
