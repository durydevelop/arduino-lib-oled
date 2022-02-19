# Driver for OLED displays with SSD1306 or SH1106


***
### This driver is distributed without license. You may use it for free.
Author: Stefan Frings, 2017

### Update by:
Author: Fabio Durigon, 2018, 2022\
email: develop@dury.it
***

## Features:
 * Supported display sizes: 196x16, 28x32 and 128x64 pixels.
 * The I2C communication is done by software bit-banging the configurable I/O pins.
 * Supports all print() and write() calls as the internal Serial lib of Arduino core.
 * Supports also standard C/C++ printf() function.
 * Added printf(x,y,...) to print directly at x,y coordinate with one call.
 * Some special characters are handled like in unix mode:
 	'\n' or '\r' -> The cursor is moved to the begin of next line.
 	'\r' and '\n' consecutive (also inverted) acts as a single one so -> The cursor is moved to the begin of next line.
 	'\f' like line-feed scroll up entire page.
 * TTY mode:
 	* Display can be used like a terminal window (without positioning the cursor before print).
 	* Any call to setCursor() has no effect.
 	* When the cursor position reach the bottom of the screen, the page is scrolled up by one line.
 * Added some wrappers for U8g2 library API compatibily. Now, for writing strings, You can use same U8g2 APIs (see [How to use](#how-to-use)):
    * drawString(Column,Row,String) ->  draw string at specific ROW and COLUMN.
    * inverse()                     ->  enable inverted font print.
    * noInverse()                   ->  disable inverted font print.
 
## Limitations:
 * This driver supports only displays with internal charge pump and I2C interface.
 * Communication errors are not handled.
 * Only one 6x8 font is supported.
 
## How to use:

### 1) Declaration
This constructor is DEPRECATED:

```C++
OLED(uint8_t sda_pin,			     // sda pin for I2C comunication
     uint8_t scl_pin,			     // scl pin for I2C comunication
	 uint8_t reset_pin=NO_RESET_PIN, // Reset pin (default: none)
	 uint8_t i2c_address=0x3C,		 // I2C address (default: 0x3C)
	 uint_fast8_t width=128,		 // Pixel width
	 uint_fast8_t height=32,		 // Pixel Height
	 bool isSH1106=false			 // Display type: true for SSD1306, false for SH1106 (default: false)  
);
```
Please Use this one:
```C++
    OLED(uint8_t sda_pin,                       // sda pin for I2C comunication
         uint8_t scl_pin,                       // scl pin for I2C comunication
	     uint8_t reset_pin=NO_RESET_PIN,        // Reset pin (default: none)
         tWidth width=W_128,                    // Display width, must be one of enum: W_96 or W_128 (default: W_128).
         tHeight height=H_32,                   // Display height, must be one of enum: H_16, H_32 or OLED::H_64 (default: H_32).
         tDisplayCtrl displayCtrl=CTRL_SSD1306, //Display controller chip, must be one of enum: CTRL_SH1106 or CTRL_SSD1306 (default: CTRL_SSD1306).
         uint8_t i2c_address=0x3C               // I2C address (default: 0x3C)
    );
```
Now, minimal instance can be:

```C++
OLED Display(4,5,16);
```
	
that means:

```C++
// Contructor used for OLED display directly mounted on NodeMCU WiFi_KIT_8 model from Heltec (http://www.heltec.cn/project/wifi-kit-8/?lang=en).
OLED Display(4,5,16,OLED::W_128,OLED::H_32,OLED::CTRL_SSD1306,0x3C);
```
&nbsp;
### 2) Initialize display lib like Serial artuino lib:

```C++
display.begin();
```
&nbsp;
### 3) Write a string in 5 different ways. All these methods will produce the same output:

```C++
// Method 1: pixel position
display.draw_string(6,8,"Hello World");

// Method 2: pixel position
display.setCursor(6,8);
display.print("Hello World");

// Method 3: pixel position
display.printf(6,8,"Hello World");

// Method 4: pixel position
display.setCursor(6,8);
display.printf("Hello World");

// Method 5: ROW and COLUMN position (U8g2 library API wrapper)
display.drawString(1,1,"Hello World");
```
### U8g2 wrappers:

Setup U8g2:
```C++
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE,19,18);
display.begin();
display.setFont(u8x8_font_chroma48medium8_r);
```
Setup this lib (no need to set font):

```C++
#include <oled.h>
OLED display(18,19,NO_RESET_PIN,OLED::W_128,OLED::H_64);
display.begin();
```
Common usage:
```C++
display.inverse();
display.drawString(0,0,"Inverted Hello World"); // first line
display.noInverse();
display.drawString(0,1,"Normal Hello World"); // second line
```
The only difference is that this lib use only one 6x8 font, so, total columns are 21 insead of 16 of U8g2 8x8 font.