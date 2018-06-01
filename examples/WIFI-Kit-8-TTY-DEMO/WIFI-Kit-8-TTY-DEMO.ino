#include <Arduino.h>
#include <oled.h>
/**
 * This example dimonstrate use of TTY mode of OLED lib
 * https://github.com/durydevelop/arduino-lib-oled.git
 * 
 * TTY mode allows You:
 * _to call some print function without refresh display
 * _to insert a '\r' or '\n' or call println() writeln() functions to automatically position cursor at begin of next line
 * 
 */

// There are 2 different versions of the board
// OLED display=OLED(2,14,4); 
OLED display=OLED(4,5,16);
int lineNr=0;

void setup()
{
    delay(1000);
    display.begin();
    display.setTTYMode(true);
    display.printf("TTY:>Press button");
}


void loop()
{
    if (digitalRead(0) == LOW) {
      lineNr++;
      
      /**
       * First Method: same as Serial.print()
       * 3 lines
       * display.print('\n');
       * display.print("TTY:>Button press ");
       * display.print(lineNr);
       */
       
      /** 
       * Second Method: use printf() 
       * ...only une line
       */
      display.printf("\nTTY:>Button press %d",lineNr);
      
      
      if (lineNr == 65535) lineNr=0;
      delay(200);
    }
}

