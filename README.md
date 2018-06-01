# Driver for OLED displays with SSD1306 or SH1106


***
This driver is distributed without license. You may use it for free.
Author: Stefan Frings, 2017

Update by:
Author: Fabio Durigon, 2018
email: develop@dury.it
***

## Futures:
 * Supported display sizes: 196x16, 28x32 and 128x64 pixels.
 * The I2C communication is done by software bit-banging the configurable I/O pins.
 * Supports all print() and write() calls as the internal Serial lib of Arduino core
 * Supports also standard C/C++ printf() function
 * Added printf(x,y,...) to print directly at x,y coordinate with one call
 * Some special characters are handled like in unix mode:
 	'\n' or '\r' -> The cursor is moved to the begin of next line
 	'\r' and '\n' consecutive (also inverted) acts as a single one so -> The cursor is moved to the begin of next line
 	'\f' like line-feed scroll up entire page.
 * TTY mode:
 	* Display can be used like a terminal window (without positioning the cursor before print).
 	* Any call to setCursor() has no effect
 	* When the cursor position reach the bottom of the screen, the page is scrolled up by one line.
 
## Limitations:

 * This driver supports only displays with internal charge pump and I2C interface.
 * Communication errors are not handled.
 
## How to use:

### Declaration
The class constructor declaration is:

```C++
	OLED(	uint8_t sda_pin,					// sda pin for I2C comunication
			uint8_t scl_pin,					// scl pin for I2C comunication
			uint8_t reset_pin=NO_RESET_PIN,	// Reset pin (default: none)
			uint8_t i2c_address=0x3C,		// I2C address (default: 0x3C)
        	uint_fast8_t width=128,			// Pixel width
        	uint_fast8_t height=32,			// Pixel Height
        	bool isSH1106=false				// Display type: true for SSD1306, false for SH1106 (default: false)  
       );
```
       
so, minimal class declarations can be:

```C++
	OLED Display=OLED(4,5,16);
```
	
that's same of:

```C++
OLED Display=OLED(4,5,16,0x3C,128,32,false);
```

That's for OLED display mounted directly on NodeMCU WiFi_KIT_8 model from Heltec (http://www.heltec.cn/project/wifi-kit-8/?lang=en) 

### Initilize display lib like Serial artuino lib:

```C++
	display.begin();
```

### Write a string:

```C++
	/* Method 1 */
	display.draw_string(0,0,"Hello World");
	
	/* Method 2 */
	display.setCursor(0,0);
	display.print("Hello World");
	
	/* Method 3 */
	display.printf(0,0,"Hello World");
	
	/* Method 4 */
	display.setCursor(0,0);
	display.printf("Hello World");
```

