# Driver for OLED displays with SSD1306 or SH1106

***
***
#### Futures:
 * Supported display sizes: 196x16, 28x32 and 128x64 pixels.
 * The I2C communication is done by software bit-banging the configurable I/O pins.
 * Supports all print() and write() calls as the internal Serial lib of Arduino core
 * Supports also standard C/C++ printf() function
 * Added printf(x,y,...) to print directly at x,y coordinate with one call
 * TTY mode:
 	* Display can be used like a terminal window (without positioning the cursor before print).
 	* Afetr call to println(), writeln() or putting '\n' or '\r' in printf(), print(), write(), so, each
 	  time that a '\r' or '\n' are processed the cursor position is moved to the beginning to the next line.
 	* Any call to setCursor() has no effect
 	* When the cursor position reach the bottom of the screen, the page is scrolled up by one line.
 
### Limitations:

 * This driver supports only displays with internal charge pump and I2C interface.
 * Communication errors are not handled.
 
#### How to use:

Initialize all stuffs

```C++
	OLED display=OLED(4,5,16); // OLED display mounted on WIFi Kit 8
	// Initilize display lib
	display.begin();
```

To write a string:

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

