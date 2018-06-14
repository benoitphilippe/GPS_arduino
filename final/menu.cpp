#include "menu.h"

#include "Bounce2.h"
#include <LiquidCrystal.h>
#include "TinyGPS.h"

int clavier() {
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  
  int bit0 = debouncer1.read();
  int bit1 = debouncer2.read();
  int enable = debouncer3.read();

  int input = enable*100+bit1*10+bit0;
  if (enable != current_enable) {
    current_enable = enable;
    switch (input) {
      case 100:
        return 1;

      case 101:
        return 2;

      case 110:
        return 3;

      case 111:
        return 4;

      default:
        return 0;
    }
  } else {
      return 0;
  }
}

void update_menu(){
    switch (input)
    {
    // Button 1 is "cancel" or "return"
    case 1:
        if (menu == fen11 | menu == fen12)
        {
            menu = fen1;
        }
        else if (menu == trip1 | menu == trip2 | menu == trip3 | menu == trip4 | menu == trip5 | menu == trip6 | menu == trip7 | menu == trip8 | menu == trip9 | menu == trip10)
        {
            menu = fen2;
        }
        // Return to memory slot selection
        else if (menu == fen32 | menu == fen33 | menu == fen34)
        {
            // Erasing previous pointer
            delete ptr_journey;
            menu = currentTrip;
            //menu=trip1;
        }
        else if (menu == fen42)
        {
            menu = fen32;
        }
        else if (menu == fen43 | menu == fen44 | menu == fen45)
        {
            menu = fen33;
        }
        else if (menu == fen46)
        {
            menu = fen34;
        }
        // Stop recording
        else if (menu == fen56)
        {
            ptr_journey->stop_recording();
            ptr_journey->save_on_EEPROM();
            menu = fen46;
        }
        break;

    // Button 2 is "up"
    case 2:
        // Preventing from jumping menus
        if (menu != fen1 & menu != fen11 & menu != trip1 & menu != fen32 & menu != fen42 & menu != fen43 & menu != fen46 & menu != fen56)
        {
            menu = int(menu) - 1;
        }
        //
        break;

    // Button 3 is "down"
    case 3:
        // Preventing from jumping menus
        if (menu != fen2 & menu != fen12 & menu != trip10 & menu != fen34 & menu != fen42 & menu != fen45 & menu != fen46 & menu != fen56)
        {
            menu = int(menu) + 1;
        }
        break;

    // Button 4 is "enter"
    case 4:
        // Preventing from jumping menus
        if (menu == fen11 | menu == fen12 | menu == fen42 | menu == fen43 | menu == fen44 | menu == fen45 | menu == fen56)
        {
            break;
        }
        // Enters "current coordinates" menu
        if (menu == fen1)
        {
            menu = fen11;
        }
        // Enters "memory slots" menu
        else if (menu == fen2)
        {
            menu = trip1;
        }
        // List of memory slots
        else if (menu == trip1 | menu == trip2 | menu == trip3 | menu == trip4 | menu == trip5 | menu == trip6 | menu == trip7 | menu == trip8 | menu == trip9 | menu == trip10)
        {
            // Saving memory slot chosen by user
            currentTrip = menu;
            // Create a journey with currentTrip ID
            ptr_journey = new Journey(currentTrip);
            menu = fen32;
        }
        // Export to USB
        else if (menu == fen32)
        {
            menu = fen42;
        }
        // Display chosen trip datas'
        else if (menu == fen33)
        {
            menu = fen43;
        }
        // New trip confirmation
        else if (menu == fen34)
        {
            menu = fen46;
        }
        // Acquisition validation
        else if (menu == fen46)
        {
            ptr_journey->start_recording();
            menu = fen56;
        }
        break;
    }
}

void do_action(){
    switch (menu)
    {
    case fen1:
        lcd.setCursor(0, 0);
        lcd.println("DISPLAY ");
        lcd.setCursor(0, 1);
        lcd.print("COORD");
        break;
    case fen11:
        lcd.setCursor(0, 0);
        // X coordinates
        lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        lcd.setCursor(0, 1);
        // Y coordinates
        lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        break;
    case fen12:
        lcd.setCursor(0, 0);
        lcd.print("Z");
        lcd.setCursor(0, 1);
        lcd.print(falt);
        break;
    case fen2:
        lcd.setCursor(0, 0);
        lcd.print("RECORDS");
        break;
    case trip1:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 1");
        break;
    case trip2:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 2");
        break;
    case trip3:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 3");
        break;
    case trip4:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 4");
        break;
    case trip5:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 5");
        break;
    case trip6:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 6");
        break;
    case trip7:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 7");
        break;
    case trip8:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 8");
        break;
    case trip9:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 9");
        break;
    case trip10:
        lcd.setCursor(0, 0);
        lcd.print("TRIP 10");
        break;
    case fen32:
        lcd.setCursor(0, 0);
        lcd.print("EXPORT");
        lcd.setCursor(0, 1);
        lcd.print("TO USB");
        break;
    case fen33:
        lcd.setCursor(0, 0);
        lcd.print("DISPLAY");
        lcd.setCursor(0, 1);
        lcd.print(" DATA ");
        break;
    case fen42:
        lcd.setCursor(0, 0);
        lcd.print("EXPORT");
        lcd.setCursor(0, 1);
        lcd.print("IN PROG");

        // Exporting datas
        ptr_journey->print_coords();
        // When finished
        lcd.setCursor(0, 1);
        menu = fen32;
        break;
    case fen43:
        lcd.setCursor(0, 0);
        lcd.print("SPEEDAVG");
        lcd.setCursor(0, 1);
        lcd.print(ptr_journey->get_m_speed(), 2);
        break;
    case fen44:
        lcd.setCursor(0, 0);
        lcd.print("LENGTH");
        lcd.setCursor(0, 1);
        lcd.println(ptr_journey->get_total_distance() / 1000.0, 2);
        break;
    case fen45:
        lcd.setCursor(0, 0);
        lcd.print("DURATION");
        lcd.setCursor(0, 1);
        lcd.println(ptr_journey->get_time() / 1000.0, 2);
        break;
    case fen34:
        lcd.setCursor(0, 0);
        lcd.print("  NEW  ");
        lcd.setCursor(0, 1);
        lcd.print(" ROUTE ");
        break;
    case fen46:
        lcd.setCursor(0, 0);
        lcd.print("BEGIN ?");
        break;
    case fen56:
        lcd.setCursor(0, 0);
        lcd.print("FETCHING");
        lcd.setCursor(0, 1);
        lcd.print(" DATA ");
        break;
    default:
        lcd.setCursor(1, 2);
        lcd.print("REBOOT");
        break;
    }
}