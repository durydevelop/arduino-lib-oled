#ifndef DDigitalButtonH
#define DDigitalButtonH

#ifdef ARDUINO
	// Arduino inlcudes
    #if ARDUINO >= 100
        #include "Arduino.h"
    #else
        #include "wiring.h"
        #include "WProgram.h"
    #endif
#else
	// RPi includes
    #include <stdint.h>
	// include lib which want to use (if both are included, pigpio is used)
    #include <pigpio.h>
    //#include <wiringPi.h>

    #ifndef __WIRING_PI_H__
        // for NowMillis()
        #include <chrono>
    #endif
#endif

// Set arduino compatibily state defines
#ifndef HIGH
    #define HIGH 1
#endif

#ifndef LOW
    #define LOW 0
#endif

#ifndef INPUT
    #define INPUT 0
#endif

#ifndef OUTPUT
    #define OUTPUT 1
#endif

#ifndef INPUT_PULLUP
    #define INPUT_PULLUP 2
#endif

class DDigitalButton{

	public:
		enum State {RELEASE,		//! Release			  STATE:	button is not in StatePressed state.
					PRESS,			//! Press			  STATE:	button is in StatePressed state.
					PRESSED,		//! Pressed			TRIGGER:	press-release after PressedMillis time.
					LONG_PRESSED,	//! Long pressed	TRIGGER:	button is kept pressed for longer than LongPressedMillis.
					LONG_PRESSING,	//! Long Pressing	  STATE:	after LONG_PRESSED, if button still pressed, the state change in LONG_PRESSING until button is released.
					DBL_PRESSED,	//! Double pressed	TRIGGER:	press-release-press within DblPressSpeedMillis time.
					DBL_PRESSING,	//! Double pressing	  STATE:	after DBL_PRESSED, if button still pressed, the state change in DBL_PRESSING until button is released.
					RELEASED		//! Released		TRIGGER:	release after PRESSED, LONG_PRESSED, DBL_PRESSED.
		};

		typedef void (*DCallback)(State);

		DDigitalButton(int DigitalPin, uint8_t PressedState = LOW, bool PullUp = true, unsigned int PressedMillis = 100, unsigned int LongPressedMillis = 1000, unsigned int DblPressSpeedMillis=100);
		void SetEventCallback(DCallback EventCallback);
		DDigitalButton::State Read(void);
		operator DDigitalButton::State();

	private:
        void SetPinMode(uint8_t pinMode);
        uint8_t ReadPin(void);
        unsigned long NowMillis(void);
		uint8_t Pin;
		State CurrState;
		State PrevState;
		uint8_t StatePressed;

		unsigned long ReleaseMs;
		unsigned long PressMs;
		unsigned long PressedMs;

		unsigned long PressedDuration;
		unsigned long DblPressSpeedDuration;
		unsigned long LongPressedDuration;
		DCallback Callback;
};
#endif
