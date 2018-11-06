#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include "Print.h"

//! Font map
static const uint8_t oled_font6x8 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, // !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
    0x00, 0x62, 0x64, 0x08, 0x13, 0x23, // %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00, // ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E, // @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C, // A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, // backslash
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x00, 0x01, 0x02, 0x04, 0x00, // '
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, // p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC, // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x00, 0x08, 0x77, 0x41, 0x00, // {
    0x00, 0x00, 0x00, 0x63, 0x00, 0x00, // ¦
    0x00, 0x00, 0x41, 0x77, 0x08, 0x00, // }
    0x00, 0x08, 0x04, 0x08, 0x08, 0x04, // ~
    0x00, 0x3D, 0x40, 0x40, 0x20, 0x7D, // ü
    0x00, 0x3D, 0x40, 0x40, 0x40, 0x3D, // Ü
    0x00, 0x21, 0x54, 0x54, 0x54, 0x79, // ä
    0x00, 0x7D, 0x12, 0x11, 0x12, 0x7D, // Ä
    0x00, 0x39, 0x44, 0x44, 0x44, 0x39, // ö
    0x00, 0x3D, 0x42, 0x42, 0x42, 0x3D, // Ö
    0x00, 0x02, 0x05, 0x02, 0x00, 0x00, // °
    0x00, 0x7E, 0x01, 0x49, 0x55, 0x73, // ß
};

// You may change the communication speed here
//#define OLED_I2C_DELAY delayMicroseconds(3) //  68kHz@80Mhz or  83kHz@160Mhz
//#define OLED_I2C_DELAY delayMicroseconds(2) //  85kHz@80Mhz or 110kHz@160Mhz
  #define OLED_I2C_DELAY delayMicroseconds(1) // 110kHz@80Mhz or 170kHz@160Mhz
//#define OLED_I2C_DELAY delayMicroseconds(0) // 150kHz@80Mhz or 300kHz@160Mhz
//#define OLED_I2C_DELAY                      // 250kHz@80Mhz or 500kHz@160Mhz

#define OLED_FONT_HEIGHT 8 //! Font height
#define OLED_FONT_WIDTH 6 //! Font witdh

#define OLED_DEFAULT_TTY_MODE false; //! Default TTY Mode

#define NO_RESET_PIN 255 //! Use as pin number if the reset line is not connected to an I/O pin. In this case, the reset signal must be generated by some other hardware.

/**
 * Driver for OLED displays with SSD1306 or SH1106 controller with write(), print(), println(), printf() support.
 *
 * Supported display sizes: 196x16, 28x32 and 128x64 pixels.
 * This driver supports only displays with internal charge pump and I2C interface.
 * The I2C communication is done by software bit-banging the configurable I/O pins.
 * Communication errors are not handled.
 *
 * The driver allocates 192, 512 or 1024 bytes memory on the heap for buffering.
 *
 * The display is technically organized into "pages" which is a set of 8 horizontal lines.
 * For a 128x34 display the memory contains 4 pages each of 128 bytes.
 * Each byte represents 8 pixels. Bit 0 is the top pixel, bit 7 is the bottom pixel.
 *
 * Bitmap graphics have the same format with the exception that they can have any width. The
 * height is always a multiple of 8.
 *
 * Text and bitmaps can be drawn to any position, but vertical positions at the
 * page boundaries (y=0, 8, 16, 24, 32, 40, 48, 56) provide better performance.
 *
 * For all drawing functions, the coordinate 0,0 is the top left corner of the display.
 * After drawing into the buffer, you have to call the display() method to send the data to
 * the display controller. Otherwise you won't see anything.
 *
 * This driver is distributed without license. You may use it for free.
 * Author: Stefan Frings, 2017
 *
 * Update by:
 * Author: Fabio Durigon, 2018
 * email: develop@dury.it
 **/
class OLED : public Print
{
public:
	//! Possible colors for drawing
    enum tColor { PIXEL_COLOR_BLACK, PIXEL_COLOR_WHITE};
    
    //! Filling mode
    enum tFillmode { HOLLOW, SOLID };
    
    //!  Supported text sizes. Normal=6x8 pixels, Double=12x16 pixels
    enum tSize { FONT_SIZE_NORMAL, FONT_SIZE_DOUBLE };
    
    //! Scroll effects supported by the display controller, note that there is no plain vertical scrolling
    enum tScrollEffect { NO_SCROLLING=0, HORIZONTAL_RIGHT=0x26, HORIZONTAL_LEFT=0x27, DIAGONAL_RIGHT=0x29, DIAGONAL_LEFT=0x2A };
    
    //! Constructor
    OLED(uint8_t sda_pin, uint8_t scl_pin, uint8_t reset_pin=NO_RESET_PIN, uint8_t i2c_address=0x3C, uint_fast8_t width=128, uint_fast8_t height=32, bool isSH1106=false);
    
    //! Destructor.
    virtual ~OLED();    
    
    //! Initialize the display controller.
    void begin();
    
    //! Enable or disable the charge pump and display output. May be used to save power.
    void set_power(bool enable);
       
    //! Enable/Disable display inverting
    void set_invert(bool enable);    
    
    //! Set the contrast (brightness) of the display.
    void set_contrast(uint8_t contrast);
    
    //! Enable continuous scrolling
    void set_scrolling(tScrollEffect scroll_type, uint_fast8_t first_page=0, uint_fast8_t last_page=7);
       
    //! Scroll up display
    void scroll_up(uint_fast8_t num_lines=OLED_FONT_HEIGHT, uint_fast8_t delay_ms=0);
    
     //! Refresh display content
    void display();    
    
    //! Clear screen content
    void clear(void);
       
    //! Draw a bitmap from RAM
    void draw_bitmap(uint_fast8_t x, uint_fast8_t y, uint_fast8_t width, uint_fast8_t height, const uint8_t* data);
    
    //! Draw a bitmap from program memory (aka FLASH)
    void draw_bitmap_P(uint_fast8_t x, uint_fast8_t y, uint_fast8_t width, uint_fast8_t height, const uint8_t* data);
    
     //! Draw a character.
    size_t draw_character(uint_fast8_t x, uint_fast8_t y, char c);
    
     //! Draw a C string from RAM, which is a NULL terminated array of characters.
    void draw_string(uint_fast8_t x, uint_fast8_t y, const char* s);
    
     //! Draw a C string from program memory (aka FLASH), which is a NULL terminated array of characters.
    void draw_string_P(uint_fast8_t x, uint_fast8_t y, const char* s);
   
     //! Draw a single pixel
    void draw_pixel(uint_fast8_t x, uint_fast8_t y);
    
    //! Draw a line
    void draw_line(uint_fast8_t x0, uint_fast8_t y0, uint_fast8_t x1, uint_fast8_t y1);
    
     //! Draw a circle
    void draw_circle(uint_fast8_t x, uint_fast8_t y, uint_fast8_t radius, tFillmode fillMode=HOLLOW);
    
    //! Draw a rectangle
    void draw_rectangle(uint_fast8_t x0, uint_fast8_t y0, uint_fast8_t x1, uint_fast8_t y1, tFillmode fillMode=HOLLOW);

    //! Write a character at current X,Y coordinates.
    size_t write(uint8_t c) override;

    /**
     * These functions are copied from HardwareSeral.h of Arduino core.
     * I implemented them to be fully compatible witgh Serial calls
     */
    inline size_t write(unsigned long n)
	{
		return write((uint8_t) n);
	}
	inline size_t write(long n)
	{
		return write((uint8_t) n);
	}
	inline size_t write(unsigned int n)
	{
		return write((uint8_t) n);
	}
	inline size_t write(int n)
	{
		return write((uint8_t) n);
	}

	//! Write a sting at current X,Y coordinates
	size_t write(const uint8_t *buffer, size_t len) override;

	//! Use also Print::write(const uint8_t *buffer, size_t size)
    using Print::write;

    //! Set cursor position
    void setCursor(uint_fast8_t x, uint_fast8_t y);

    //! Same as printf width x and y coordinate set
    size_t printf(uint_fast8_t x, uint_fast8_t y, const char *format, ...);

    //! Use also Print::printf(const char *format, ...)
    using Print::printf;

    //! Set terminal mode on/off
    void setTTYMode(bool Enabled);

    //! Change current font size
    void setFontSize(tSize size);

    //! Change current font and drawing color
    void setPixelColor(tColor Color);
        
private:
    const uint8_t sda_pin; //! Pin for the SDA line
    
    const uint8_t scl_pin; //! Pin for the SCL line

    const uint8_t reset_pin; //! Pin for the /RST line (value=NO_RESET_PIN, is disabled)
    
    const uint_fast8_t width; //! Horizontal display size in pixels
    
    const uint_fast8_t height; //! Vertical display size in pixels
    
    const uint8_t i2c_address; //! I2C address of the display controller
    
    const bool isSH1106; //! true=SH1106 controller, false=SSD1306 controller
    
    const uint_fast8_t pages; //! Number of pages in the display and buffer
       
    const uint_fast16_t bufsize; //! Size of the buffer in bytes
    
    uint8_t *buffer; //! Pointer to the buffer memory
    
    uint_fast8_t X; //! Current X position

	uint_fast8_t Y; //! Current Y position

	tSize fontSize; //! Current font size, can be NORMAL_SIZE or DOUBLE_SIZE

	tColor pixelColor; //! Current pixels color, can be BLACK or WHITE

	bool ttyMode; //! TTY mode

	//! Draw a byte into the buffer
	void draw_byte(uint_fast8_t x, uint_fast8_t y, uint8_t b);

	//! Draw multiple bytes into the buffer
	void draw_bytes(uint_fast8_t x, uint_fast8_t y, const uint8_t* data, uint_fast8_t size, bool useProgmem);
    
	//! Send an I2C start signal
	void i2c_start();

	//! Send an I²C stop signal
	void i2c_stop();
    
	//! Send a byte via I2C and return the ack
    bool i2c_send(uint8_t byte);
};

#endif /* OLED_H */

