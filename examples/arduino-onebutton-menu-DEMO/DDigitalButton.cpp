/*! \mainpage
*	Arduino and Raspberry Pi library for handle button press.
* \section intro_sec Introduction
* This library can be used with Arduino or Raspberry Pi, hardware specific api are performed by pre-processor defines.
*
* Futures:
* -Arduino compliant.
* -Raspberry Pi compliant. Both WiringPi and Pigpio library are supported (see includes comments in DDigitalButton.h)
* -In polling mode, all 8 states and triggers can be read and handled:
* -In event mode, 4 triggers fires callback.
*
* State and triggers:
	RELEASE			STATE:		button is not in StatePressed state.
	PRESS			STATE:		button is in StatePressed state.
	PRESSED			TRIGGER:	press-release after PressedMillis time.
	LONG_PRESSED	TRIGGER:	button is kept pressed for longer than LongPressedMillis.
	LONG_PRESSING	STATE:		after LONG_PRESSED, if button still pressed, the state change in LONG_PRESSING until button is released.
	DBL_PRESSED		TRIGGER:	press-release-press within DblPressSpeedMillis time.
	DBL_PRESSING	STATE:		after DBL_PRESSED, if button still pressed, the state change in DBL_PRESSING until button is released.
	RELEASED		TRIGGER:	release after PRESSED, LONG_PRESSED, DBL_PRESSED.
*
* \section usage_sec Usage
*
* @code
*
* @endcode
*/

#include "DDigitalButton.h"

/**
 * @brief Constructor
 * @param DigitalPin			->	Pin number where button is connected.
 * @param PressedState			->	Can be HIGH or LOW and it is the pin level for which the button is considered to be pressed.
 * @param PullUp				->	If true the internal pull-up resistor is connected.
 * @param PressedMillis			->	Time in milliseconds after which "press and release" is triggered as PRESSED.
 * @param LongPressedMillis		->	Time in milliseconds after which a "keeping press" is triggered as LONG_PRESSED.
 * @param DblPressSpeedMillis	->	Time in milliseconds before which "press, release, press" is triggered as DBL_PRESSED.
 */
DDigitalButton::DDigitalButton(int DigitalPin, uint8_t PressedState, bool PullUp, unsigned int PressedMillis, unsigned int LongPressedMillis, unsigned int DblPressSpeedMillis)
{
	// User settings
	Pin=DigitalPin;
	StatePressed=PressedState;
	PressedDuration=PressedMillis;
	LongPressedDuration=LongPressedMillis;
	DblPressSpeedDuration=DblPressSpeedMillis;

	// Init timers
	ReleaseMs=NowMillis();
	PressMs=ReleaseMs;
	PressedMs=ReleaseMs;

	// Trigger Callback
	Callback=NULL;

	// Pull-up
	if (PullUp)	{
		SetPinMode(INPUT_PULLUP);
	}
	else {
		SetPinMode(INPUT);
	}

	// Read current input state
	Read();
}

/**
 * Enable event mode: set a callback that is called on each TRIGGER state.
 * @param EventCallback	->	Function to call.
 * Callback function must be used in this way:
 * @code
 * void EventCallback(DDigitalButton::State trigger) {
 * 		if (trigger == DDigitalButton::PRESSED) {
 * 			// Do something
 * 		}
 * }
 */
void DDigitalButton::SetEventCallback(DCallback EventCallback)
{
	Callback=EventCallback;
}

//! @return the current button State.
DDigitalButton::State DDigitalButton::Read(void)
{
	bool Trig=false;

	if (PrevState == RELEASED) {
		CurrState=RELEASE;
	}
	if (PrevState == PRESSED) {
		CurrState=PRESS;
	}

	if (ReadPin() == StatePressed) {
		PressMs=NowMillis(); // Press time
		if (PrevState == RELEASE) {
			if ((PressMs-ReleaseMs) > 50) {
				CurrState=PRESS;
			}
		}
		else if (PrevState == PRESS) {
			if ((PressMs-PressedMs) < DblPressSpeedDuration) {
				CurrState=DBL_PRESSED;
				Trig=true;
			}
			if ((PressMs-ReleaseMs) > LongPressedDuration) {
				CurrState=LONG_PRESSED;
				Trig=true;
			}
		}
		else if (PrevState == LONG_PRESSED) {
			CurrState=LONG_PRESSING;
		}
		else if (PrevState == DBL_PRESSED) {
			CurrState=DBL_PRESSING;
		}
	}
	else {
		ReleaseMs=NowMillis(); // Release time
		if (PrevState == PRESS) {
			if ((ReleaseMs-PressMs) > PressedDuration) {
				CurrState=PRESSED;
				Trig=true;
				PressMs=ReleaseMs;
			}
			else if ((ReleaseMs-PressMs) > 50) {
				PressedMs=NowMillis();
			}
		}
		else if (PrevState == PRESSED || PrevState == LONG_PRESSING || PrevState == LONG_PRESSED) {
			CurrState=RELEASED;
			Trig=true;
		}
		else {
			CurrState=RELEASE;
		}
	}

	PrevState=CurrState;
	if (Trig) {
		if (Callback != NULL) {
			//Serial.println("Callback");
			Callback(CurrState);
		}
	}

	return (CurrState);
}

//! Overload operator ==
DDigitalButton::operator DDigitalButton::State()
{
	return Read();
}

//! Wrapper function to set mode for pin (INPUT or INPUT_PULLUP)
void DDigitalButton::SetPinMode(uint8_t Mode)
{
    #ifdef ARDUINO
        pinMode(Pin,Mode);
    #else
        #ifdef PIGPIO_VERSION
            gpioSetMode(Pin,Mode);
        #else
            pinMode(Pin,Mode);
        #endif
    #endif
}

//! Wrapper function to read direct pin level.
/**
 * @return pin level (HIGH or LOW)
 */
uint8_t DDigitalButton::ReadPin() {
    #ifdef ARDUINO
		return(digitalRead(Pin));
    #else
        #ifdef PIGPIO_VERSION
			return(gpioRead(Pin));
        #else
			return(digitalRead(Pin));
        #endif
    #endif
}

//! Wrapper function for getting arduino "millis()" function value
unsigned long DDigitalButton::NowMillis(void)
{
    #ifdef ARDUINO
        return(millis());
    #else
        #ifdef PIGPIO_VERSION
            // c++ equivalent of arduino millis()
            //extern unsigned int millis            (void) ;
            return(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        #else
            return(millis());
        #endif
    #endif
}

/*
long millis(){
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return _t.tv_sec*1000 + lround(_t.tv_nsec/1.0e6);
}
*/
