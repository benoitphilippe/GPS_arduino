#ifndef MENU_H
#define MENU_H

#include "Bounce2.h"
#include "journey.h"
#include <LiquidCrystal.h>
#include "TinyGPS.h"

enum Fenetres {
  trip1, trip2, trip3, trip4, trip5, trip6, trip7, trip8, trip9, trip10, fen1, fen2, fen11, fen12, fen13, fen14, fen32, fen33, fen34, fen42, fen43, fen44, fen45, fen46, fen56
};

extern Fenetres menu;
extern int input;

// Debouncers
extern Bounce debouncer1;
extern Bounce debouncer2;
extern Bounce debouncer3;
// global vars
extern int current_enable;
extern Journey * ptr_journey;
extern int currentTrip;
extern  LiquidCrystal lcd;

// fonctions
int clavier();
void update_menu();
void do_action();
#endif
