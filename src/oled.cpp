#include <Arduino.h>
#include "oled.h"

/**
 * \example WIFI-Kit-8-DEMO.ino
 * \example WIFI-Kit-8-TTY-DEMO.ino
 * \example WIFI-Kit-8-WiFiScan.ino
 */

/**
* Constructor of the OLED class.
* @param sda_pin Pin number of the SDA line (can be any Arduino I/O pin)
* @param sda_pin Pin number of the SCL line (can be any Arduino I/O pin)
* @param reset_pin Pin number for the /RST line, or use NO_RESET_PIN if the reset signal is generated somewhere else
* @param i2c_address The I²C address is usually 0x3c or 0x3D
* @param width With of the display in pixels: 96 or 128
* @param height Height of the display in pixels: 16, 32 or 64
* @param isSH1106 Must be true=SH1106 chip, false=SSD1306 chip
*/
OLED::OLED(uint8_t sda_pin, uint8_t scl_pin, uint8_t reset_pin, uint8_t i2c_address, uint_fast8_t width, uint_fast8_t height, bool isSH1106)
: sda_pin(sda_pin),
scl_pin(scl_pin),
reset_pin(reset_pin),
width(width),
height(height),
i2c_address(i2c_address),
isSH1106(isSH1106),
pages((height + 7) / 8),
bufsize(static_cast<uint_least16_t> (pages*width))
{
    this->buffer = (uint8_t *) malloc(bufsize); // two dimensional array of n pages each of n columns.
    // Set dafault
    X=0;
    Y=0;
    ttyMode=OLED_DEFAULT_TTY_MODE;
    fontSize=FONT_SIZE_NORMAL;
    pixelColor=PIXEL_COLOR_WHITE;
}

/**
* Frees the buffer memory and leaves the display in the previous state.
*/
OLED::~OLED()
{
    free(buffer);
}

/**
*  Initialize the display controller, clean memory and switch output on.
*/
void OLED::begin()
{
#if defined OUTPUT_OPEN_DRAIN
    pinMode(sda_pin, OUTPUT_OPEN_DRAIN);
    digitalWrite(sda_pin, HIGH);
    pinMode(scl_pin, OUTPUT_OPEN_DRAIN);
    digitalWrite(scl_pin, HIGH);
#else
    pinMode(sda_pin, INPUT);
    pinMode(scl_pin, INPUT);
#endif
    if (reset_pin != NO_RESET_PIN)
    {
        pinMode(reset_pin, OUTPUT);
        digitalWrite(reset_pin, LOW);
        delay(10);
        digitalWrite(reset_pin, HIGH);
    }
    delay(100);
    i2c_start();
    i2c_send(i2c_address << 1); // address + write
    i2c_send(0x00); // command
    i2c_send(0xAE); // display off
    i2c_send(0xD5); // clock divider
    i2c_send(0x80); 
    i2c_send(0xA8); // multiplex ratio
    i2c_send(height - 1); 
    i2c_send(0xD3); // no display offset
    i2c_send(0x00); 
    i2c_send(0x40); // start line address=0
    i2c_send(0x8D); // enable charge pump
    i2c_send(0x14); 
    i2c_send(0x20); // memory adressing mode=horizontal
    i2c_send(0x00); 
    i2c_send(0xA1); // segment remapping mode
    i2c_send(0xC8); // COM output scan direction
    if (width == 128 && height == 32)
    {
        i2c_send(0xDA); // com pins hardware configuration
        i2c_send(0x02); 
    }
    else if (width == 128 && height == 64)
    {
        i2c_send(0xDA); // com pins hardware configuration
        i2c_send(0x12); 
    }
    else if (width == 96 && height == 16)
    {
        i2c_send(0xDA); // com pins hardware configuration
        i2c_send(0x02); 
    }
    i2c_send(0x81); // contrast control
    i2c_send(0x80);
    i2c_send(0xD9); // pre-charge period
    i2c_send(0x22); 
    i2c_send(0xDB); // set vcomh deselect level
    i2c_send(0x20); 
    i2c_send(0xA4); // output RAM to display
    i2c_send(0xA6); // display mode A6=normal, A7=inverse
    i2c_send(0x2E); // stop scrolling
    i2c_stop();
    delay(100);

    // Clear and send buffer
    clear();
    display();

    // Switch display on
    set_power(true);
}

/**
* This command is executed by the display controller itself, hence it does not affect the buffer memory.
* @param enable Whether to enable the display output.
*/
void OLED::set_power(bool enable)
{
    i2c_start();
    i2c_send(i2c_address << 1); // address + write
    i2c_send(0x00); // command
    if (enable)
    {
        i2c_send(0x8D); // enable charge pump
        i2c_send(0x14);
        i2c_send(0xAF); // display on
    }
    else
    {
        i2c_send(0xAE); // display off
        i2c_send(0x8D); // disable charge pump
        i2c_send(0x10);
    }
    i2c_stop();
}

/**
* If enabled, then WHITE and BLACK are swapped.
* This command is executed by the display controller itself, so it works very fast and does not affect
* the buffer memory.
* @param enable Whether to enable inverse output
*/
void OLED::set_invert(bool enable)
{
    i2c_start();
    i2c_send(i2c_address << 1); // address + write
    i2c_send(0x00); // command
    if (enable)
    {
        i2c_send(0xA7); // invert display
    }
    else
    {
        i2c_send(0xA6); // normal display
    }
    i2c_stop();
}

/**
* The value affects the electrical current throught the OLED segments in linear ratio.
* But the eye reacts logarithmically. A medium brightnes of 128 looks almost as bright as the
* maximum setting but increases the lifetime of the display a lot.
* @param contrast Contrast value, default is 0x80.
**/
void OLED::set_contrast(uint8_t contrast)
{
    i2c_start();
    i2c_send(i2c_address << 1); // address + write
    i2c_send(0x00); // command
    i2c_send(0x81); // deativate scrol
    i2c_send(contrast);
    i2c_stop();
}

/**
* This is performed by the display controller itself without needing continuous
* communication and it does not affect the buffer memory.
* For horizontal scrolling, you can decide whether the whole display or only a part of it shall scroll.
* In case of diagonal scrolling, the vertical part affects always the whole display.
* @param scroll_type Select the scroll effect, or NO_SCROLLING to disable it
* @param first_page Defined which pages are affected by the horizontal scrolling (0-7). Each page contains 8 lines
* @param last_page Defined which pages are affected by the horizontal scrolling (0-7). Each page contains 8 lines
**/
void OLED::set_scrolling(tScrollEffect scroll_type, uint_fast8_t first_page, uint_fast8_t last_page)
{
    i2c_start();
    i2c_send(i2c_address << 1); // address + write
    i2c_send(0x00); // command
    i2c_send(0x2E); // deativate scroll
    if (scroll_type == DIAGONAL_LEFT || scroll_type == DIAGONAL_RIGHT)
    {
        i2c_send(0xA3); // vertical scroll area
        i2c_send(0x00);
        i2c_send(height - 1);
    }
    if (scroll_type != NO_SCROLLING)
    {
        i2c_send(scroll_type);
        i2c_send(0x00); // dummy byte
        i2c_send(first_page);
        i2c_send(0x00); // time interval
        i2c_send(last_page);
        if (scroll_type == DIAGONAL_LEFT || scroll_type == DIAGONAL_RIGHT)
        {
            i2c_send(0x01); // vertical scrolling speed
        }
        else
        {
            i2c_send(0x00); // dummy byte
            i2c_send(0xFF); // dummy byte
        }
        i2c_send(0x2F); // ativate scroll
    }
    i2c_stop();
}

/**
     * Scroll the display up, which leaves a number of black pixel lines at the bottom.
     * An optional delay time can be used to produce a smooth scrolling effect.
     * This is done by software, requiring continuous communication.
     * @param num_lines Number of pixel lines to scroll the display content up.
     * @param delay_ms Delay time between each step, recommended values are either 0 or 20..60.
     */
void OLED::scroll_up(uint_fast8_t num_lines, uint_fast8_t delay_ms)
{
    if (delay_ms == 0)
    {
        // Scroll full pages, fast algorithm
        uint_fast8_t scroll_pages = num_lines / 8;
        for (uint_fast8_t i = 0; i < pages; i++)
        {
            for (uint_fast8_t x = 0; x < width; x++)
            {
                uint16_t index = i * width + x;
                uint16_t index2 = (i + scroll_pages) * width + x;
                if (index2 < bufsize)
                {
                    buffer[index] = buffer[index2];
                }
                else
                {
                    buffer[index] = 0;
                }
            }
        }
        num_lines -= scroll_pages * 8;
    }

    // Scroll the remainder line by line
    bool need_refresh=true;
    if (num_lines > 0)
    {
        uint16_t start=millis() & 0xFFFF;
        uint16_t target_time=0;

        for (uint_fast8_t i = 0; i < num_lines; i++)
        {

            // Scroll everything 1 line up
            for (uint_fast8_t j = 0; j < pages; j++)
            {
                uint16_t index = j*width;
                uint16_t index2 = index + width;
                for (uint_fast8_t x = 0; x < width; x++)
                {
                    uint_fast8_t carry = 0;
                    if (index2 < bufsize)
                    {
                        if (buffer[index2] & 1)
                        {
                            carry = 128;
                        }
                    }
                    buffer[index] = (buffer[index] >> 1) | carry;
                    index++;
                    index2++;
                }
            }
            need_refresh=true;
            target_time+=delay_ms;

            // Refresh the display only if we have some time
            uint16_t now=millis() & 0xFFFF;
            if (now-start < target_time)
            {
                display();
                need_refresh=false;
            }

            // If we have still more time, then yield a while
            while((millis() & 0xFFFF)-start < target_time)
            {
                yield();
            }
        }
    }

    if (need_refresh)
    {
        display();
    }
}

/**
* Transfer the buffer memory data to the display controller.
* You have to call this method after any drawing action into the buffer to make the change visible.
* A call to this method takes several milliseconds.
*/
void OLED::display()
{
    uint16_t index = 0;
    for (uint_fast8_t page = 0; page < pages; page++)
    {
        // Set memory address to fill
        i2c_start();
        i2c_send(i2c_address << 1); // address + write
        i2c_send(0x00); // command
        if (isSH1106)
        {
            i2c_send(0xB0 + page); // set page
            i2c_send(0x00); // lower columns address =0
            i2c_send(0x10); // upper columns address =0
        }
        else
        {
            i2c_send(0xB0 + page); // set page
            i2c_send(0x21); // column address
            i2c_send(0x00); // first column =0
            i2c_send(width - 1); // last column
        }
        i2c_stop();

        // send one page of buffer to the display
        i2c_start();
        i2c_send(i2c_address << 1); // address + write
        i2c_send(0x40); // data
        for (uint_fast8_t column = 0; column < width; column++)
        {
            i2c_send(buffer[index++]);
        }
        i2c_stop();
        yield(); // to avoid that the watchdog triggers
    }
}

/**
* Fill the whole buffer memory with current color
*/
void OLED::clear(void)
{
    if (pixelColor == PIXEL_COLOR_WHITE)
    {
        memset(buffer, 0xFF, bufsize);
    }
    else
    {
        memset(buffer, 0x00, bufsize);
    }
    X=0;
    Y=0;
}

/**
* Draw a bitmap from RAM. The raw data format is explained in the description of this class.
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param width Width of the bitmap in pixels
* @param height Height of the bitmap in pixels. Must be a multiple of 8
* @param data Raw data, number of bytes must be width*height/8
* @param color Color to draw with
*/
void OLED::draw_bitmap(uint_fast8_t x, uint_fast8_t y, uint_fast8_t bitmap_width, uint_fast8_t bitmap_height, const uint8_t* data)
{
    uint_fast8_t num_pages = (bitmap_height + 7) / 8;
    for (uint_fast8_t page = 0; page < num_pages; page++)
    {
        draw_bytes(x, y, data, bitmap_width,false);
        data += bitmap_width;
        y += 8;
    }
}

/**
* Draw a bitmap from program memory (aka FLASH). The raw data format is explained in the description of this class.
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param width Width of the bitmap in pixels
* @param height Height of the bitmap in pixels. Must be a multiple of 8
* @param data Raw data, number of bytes must be width*height/8
* @param color Color to draw with
*/
void OLED::draw_bitmap_P(uint_fast8_t x, uint_fast8_t y, uint_fast8_t bitmap_width, uint_fast8_t bitmap_height, const uint8_t* data)
{
    uint_fast8_t num_pages = (bitmap_height + 7) / 8;
    for (uint_fast8_t page = 0; page < num_pages; page++)
    {
        draw_bytes(x,y,data,bitmap_width,true);
        data += bitmap_width;
        y += 8;
    }
}

/**
* Draw a character.
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param c The character code. Supports US-ASCII characters and german umlauts. See source code of oled.cpp
* @param color Color to draw with
*/
size_t OLED::draw_character(uint_fast8_t x, uint_fast8_t y, char c)
{
    // Invalid position
    if (x >= width || y >= height || c < 32)
    {
        return 0;
    }

    // Remap extended Latin1 character codes
    switch ((unsigned char) c)
    {
        case 252 /* u umlaut */:
            c = 127;
            break;
        case 220 /* U umlaut */:
            c = 128;
            break;
        case 228 /* a umlaut */:
            c = 129;
            break;
        case 196 /* A umlaut */:
            c = 130;
            break;
        case 246 /* o umlaut */:
            c = 131;
            break;
        case 214 /* O umlaut */:
            c = 132;
            break;
        case 176 /* degree   */:
            c = 133;
            break;
        case 223 /* szlig    */:
            c = 134;
            break;
    }

    uint16_t font_index = (c - 32)*6;

    // Invalid character code/font index
    if (font_index >= sizeof (oled_font6x8))
    {
        return 0;
    }

    draw_bytes(x, y, &oled_font6x8[font_index],6,true);
    return 1;
}

/**
* Draw a C string from RAM, which is a NULL terminated array of characters.
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param s The string to draw. Supports US-ASCII characters and german umlauts. See source code of oled.cpp
* @param color Color to draw with
*/
void OLED::draw_string(uint_fast8_t x, uint_fast8_t y, const char* s)
{
    while (*s)
    {
        draw_character(x,y,*s);
        if (fontSize == FONT_SIZE_DOUBLE)
        {
            x += 12;
        }
        else // NORMAL_SIZE
        {
            x += 6;
        }
        s++;
    }
}

/**
* Draw a C string from program memory (aka FLASH), which is a NULL terminated array of characters.
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param s The string to draw. Supports US-ASCII characters and german umlauts. See source code of oled.cpp
* @param color Color to draw with
*/
void OLED::draw_string_P(uint_fast8_t x, uint_fast8_t y, const char* s)
{
    char c;
    while ((c = pgm_read_byte(s)))
    {
        draw_character(x,y,c);
        if (fontSize == FONT_SIZE_DOUBLE)
        {
            x += 12;
        }
        else // NORMAL_SIZE
        {
            x += 6;
        }
        s++;
    }
}

/**
* Draw a single pixel.
* @param x,y Position of the pixel
* @param color Color to draw with
*/
void OLED::draw_pixel(uint_fast8_t x, uint_fast8_t y)
{
    if (x >= width || y >= height)
    {
        return;
    }
    if (pixelColor == PIXEL_COLOR_WHITE)
    {
        buffer[x + (y / 8) * width] |= (1 << (y & 7)); // set bit 
    }
    else
    {
        buffer[x + (y / 8) * width] &= ~(1 << (y & 7)); // clear bit
    }
}

/**
* Draw a line.
* @param x0,y0 Start point of the line
* @param x1,y1 End point of the line
* @param color Color to draw with
*/
void OLED::draw_line(uint_fast8_t x0, uint_fast8_t y0, uint_fast8_t x1, uint_fast8_t y1)
{        
    // Algorithm copied from Wikipedia
    int_fast16_t dx = abs(static_cast<int_fast16_t>(x1) - static_cast<int_fast16_t>(x0));
    int_fast16_t sx = x0 < x1 ? 1 : -1;
    int_fast16_t dy = -abs(static_cast<int_fast16_t>(y1) - static_cast<int_fast16_t>(y0));
    int_fast16_t sy = y0 < y1 ? 1 : -1;
    int_fast16_t err = dx + dy;
    int_fast16_t e2;

    while (1)
    {
        draw_pixel(x0, y0);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = 2 * err;
        if (e2 > dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

/**
* Draw a circle.
* @param x,y Center position of the circle
* @param radius Radius of the circle
* @param fillMode Whether the circle is filled with the color
* @param color Color to draw with
*/
void OLED::draw_circle(uint_fast8_t x0, uint_fast8_t y0, uint_fast8_t radius, tFillmode fillMode)
{
    // Algorithm copied from Wikipedia
    int_fast16_t f = 1 - radius;
    int_fast16_t ddF_x = 0;
    int_fast16_t ddF_y = -2 * radius;
    int_fast16_t x = 0;
    int_fast16_t y = radius;

    if (fillMode == SOLID)
    {
        draw_pixel(x0, y0 + radius);
        draw_pixel(x0, y0 - radius);
        draw_line(x0 - radius, y0, x0 + radius, y0);
    }
    else
    {
        draw_pixel(x0, y0 + radius);
        draw_pixel(x0, y0 - radius);
        draw_pixel(x0 + radius, y0);
        draw_pixel(x0 - radius, y0);
    }

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        if (fillMode == SOLID)
        {
            draw_line(x0 - x, y0 + y, x0 + x, y0 + y);
            draw_line(x0 - x, y0 - y, x0 + x, y0 - y);
            draw_line(x0 - y, y0 + x, x0 + y, y0 + x);
            draw_line(x0 - y, y0 - x, x0 + y, y0 - x);
        }
        else
        {
            draw_pixel(x0 + x, y0 + y);
            draw_pixel(x0 - x, y0 + y);
            draw_pixel(x0 + x, y0 - y);
            draw_pixel(x0 - x, y0 - y);
            draw_pixel(x0 + y, y0 + x);
            draw_pixel(x0 - y, y0 + x);
            draw_pixel(x0 + y, y0 - x);
            draw_pixel(x0 - y, y0 - x);
        }
    }
}

/**
* Draw a rectangle.
* @param x0,y0 Upper left corner
* @param x1,y1 Lower right corner
* @param fillMode Whether the rectangle is filled with the color
* @param color Color to draw with
*/
void OLED::draw_rectangle(uint_fast8_t x0, uint_fast8_t y0, uint_fast8_t x1, uint_fast8_t y1, tFillmode fillMode)
{
    // Swap x0 and x1 if in wrong order
    if (x0 > x1)
    {
        uint_fast8_t tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    // Swap y0 and y1 if in wrong order
    if (y0 > y1)
    {
        uint_fast8_t tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    if (fillMode == SOLID)
    {        
        for (uint_fast8_t y = y0; y <= y1; y++)
        {
            draw_line(x0, y, x1, y);
        }
    }
    else
    {
        draw_line(x0, y0, x1, y0);
        draw_line(x0, y1, x1, y1);
        draw_line(x0, y0, x0, y1);
        draw_line(x1, y0, x1, y1);
    }
}

/**
* Write a character at current X,Y coordinates.
* X coordinate will be moved by OLED_FONT_WIDTH value
* * If the string contains a \r or \n and ttyMode is true, Y coordinate will be incremented
* by OLED_FONT_HEIGHT value
* This function overrides write(...) in Print.cpp of Arduino core
*
* @param c character to write
* @return 1 on succeed, otherwise 0.
*/
size_t OLED::write(uint8_t c)
{
	int n=1;
	n=draw_character(X,Y,c);
	X+=OLED_FONT_WIDTH;
	return n;
}

/**
* Write a sting at current X,Y coordinates.
* X coordinate will be moved by (size*OLED_FONT_WIDTH) pixels.
* If ttyMode is true display() will be called to redraw screen.
* This function overrides write(...) in Print.cpp of Arduino core
* @param buffer The string to be printed
* @param size The length of the string
* @return Nr of characters written.
*/
size_t OLED::write(const uint8_t *buffer, size_t len)
{
    //size_t n = 0;
    for (size_t ix=0;ix<len;ix++)
    {
		// If CR and LF are found consecutive, only one is processed
    	// If two or more CR are consecutive, both are processed
    	// If two or more LF are consecutive, both are processed
    	if (buffer[ix] == '\r')
		{
			Serial.printf("n=%d r",ix);
    		X=0;
			Y+=(OLED_FONT_HEIGHT);
			if (buffer[ix+1] == '\n') {
				Serial.print(" + n");Serial.println();
				ix++; // skip char
			}
			Serial.printf("n=%d",ix);
			Serial.println();
		}
		else if (buffer[ix] == '\n')
		{
			Serial.printf("n=%d n",ix);
			X=0;
			Y+=(OLED_FONT_HEIGHT);
			if (buffer[ix+1] == '\r') {
				Serial.print(" + r");Serial.println();
				ix++; // skip char
			}
			Serial.printf("n=%d",ix);
			Serial.println();
		}
		else if (*(buffer+ix) == '\f')
		{
			// FORM FEED
			scroll_up(height);
			X=0;
			Y=0;
		}
		else
		{
			write(buffer[ix]);
		}

		if (ttyMode)
		{
			// Scroll up if cursor position is out of screen
			if (Y >= height)
			{
				scroll_up(OLED_FONT_HEIGHT);
				Y=height-OLED_FONT_HEIGHT;
			}
		}
    }
    if (ttyMode) display();
    return len;
}

/**
* Set cursor position
*
* @param x	->	new x coordinate
* @param y	->	new y coordinate
*/
void OLED::setCursor(uint_fast8_t x, uint_fast8_t y)
{
	if (ttyMode) return; // in TTY mode position the cursor has no effect
	X=x;
	Y=y;
}

/**
* printf func width x and y coordinate set
*
* @param x Pixel position of the upper left corner
* @param y Pixel position of the upper left corner
* @param format Format string same in printf
* @param ... Args for formatted string
*
* @return Len of written string
*/
size_t OLED::printf(uint_fast8_t x, uint_fast8_t y, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char* buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1) {
        buffer = new char[len + 1];
        if (!buffer) {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    X=x;
    Y=y;
    len = write((const uint8_t*) buffer, len);
    if (buffer != temp) {
        delete[] buffer;
    }

    return len;
}

/**
* Set terminal mode on/off
* @param Enabled If true, driver automatically scroll-up to one text line (height of font) when
* a printing function (that doesn't use coordinate) reach the bottom of the screen.
* Es.
* When the cursor Y position is at the last text line and you use println() or writeln(),
* the text on the the screen scrolls up and first line of text disappears
*/
void OLED::setTTYMode(bool Enabled)
{
	ttyMode=Enabled;
}

/**
* Change current font size
* @param size	->	available fon sizes: FONT_SIZE_NORMAL, FONT_SIZE_DOUBLE
*/
void OLED::setFontSize(tSize size)
{
	fontSize=size;
}

/**
* Change current font and drawing color
* @param size	->	available fon sizes: PIXEL_COLOR_BLACK, PIXEL_COLOR_WHITE
*/
void OLED::setPixelColor(tColor Color)
{
	pixelColor=Color;
}

/**
* Draw a byte into the buffer at specifics coordinates
* @param x	->	x coordinate
* @param y	->	y coordinate
* @param b	->	byte to write
*/
void OLED::draw_byte(uint_fast8_t x, uint_fast8_t y, uint8_t b)
{
    // Invalid position
    if (x >= width || y >= height)
    {
        return;
    }

    uint_fast16_t buffer_index = y / 8 * width + x;

    if (pixelColor == PIXEL_COLOR_WHITE)
    {
        // If the y position matches a page, then it goes quicker
        if (y % 8 == 0)
        {
            if (buffer_index < bufsize)
            {
                buffer[buffer_index] |= b;
            }
        }
        else
        {
            uint16_t w = (uint16_t) b << (y % 8);
            if (buffer_index < bufsize)
            {
                buffer[buffer_index] |= (w & 0xFF);
            }
            uint16_t buffer_index2 = buffer_index + width;
            if (buffer_index2 < bufsize)
            {
                buffer[buffer_index2] |= (w >> 8);
            }
        }
    }
    else
    {
        // If the y position matches a page, then it goes quicker
        if (y % 8 == 0)
        {
            if (buffer_index < bufsize)
            {
                buffer[buffer_index] &= ~b;
            }
        }
        else
        {
            uint16_t w = (uint16_t) b << (y % 8);
            if (buffer_index < bufsize)
            {
                buffer[buffer_index] &= ~(w & 0xFF);
            }
            uint16_t buffer_index2 = buffer_index + width;
            if (buffer_index2 < bufsize)
            {
                buffer[buffer_index2] &= ~(w >> 8);
            }
        }
    }
    return;
}

/**
* Draw multiple bytes into the buffer
* @param x			->	x coordinate
* @param y			->	y coordinate
* @param data		->	pointer to a buffer to write
* @param size		->	size of buffer
* @param useProgram	->	if true use internal mem
*/
void OLED::draw_bytes(uint_fast8_t x, uint_fast8_t y, const uint8_t* data, uint_fast8_t size, bool useProgmem)
{
    for (uint_fast8_t column = 0; column < size; column++)
    {
        uint8_t b;
        if (useProgmem)
        {
            b = pgm_read_byte(data);
        }
        else
        {
            b = *data;
        }
        data++;
        if (fontSize == FONT_SIZE_DOUBLE)
        {
            // Stretch vertically
            uint16_t w = 0;
            for (uint_fast8_t bit = 0; bit < 7; bit++)
            {
                if (b & (1 << bit))
                {
                    uint_fast8_t pos = bit << 1;
                    w |= ((1 << pos) | (1 << (pos + 1)));
                }
            }

            // Output 2 times to strech hozizontally
            draw_byte(x, y, w & 0xFF);
            draw_byte(x, y + 8, (w >> 8));
            x++;
            draw_byte(x, y, w & 0xFF);
            draw_byte(x, y + 8, (w >> 8));
            x++;
        }
        else // NORMAL_SIZE
        {
            draw_byte(x++, y, b);
        }
    }
}

void OLED::i2c_start()
{
    while (!digitalRead(sda_pin) || !digitalRead(scl_pin));
    digitalWrite(sda_pin, LOW);
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(sda_pin, OUTPUT);
#endif
    OLED_I2C_DELAY;
    digitalWrite(scl_pin, LOW);
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(scl_pin, OUTPUT);
#endif
    OLED_I2C_DELAY;
}

void OLED::i2c_stop()
{
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(scl_pin, INPUT);
#endif
    digitalWrite(scl_pin, HIGH);
    OLED_I2C_DELAY;
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(sda_pin, INPUT);
#endif
    digitalWrite(sda_pin, HIGH);
    OLED_I2C_DELAY;
    while (!digitalRead(sda_pin) || !digitalRead(scl_pin));
}

bool OLED::i2c_send(uint8_t byte)
{
    for (int_fast8_t bit = 7; bit >= 0; bit--)
    {
        if (byte & 128)
        {
#if !defined OUTPUT_OPEN_DRAIN
            pinMode(sda_pin, INPUT);
#endif
            digitalWrite(sda_pin, HIGH);
        }
        else
        {
            digitalWrite(sda_pin, LOW);
#if !defined OUTPUT_OPEN_DRAIN
            pinMode(sda_pin, OUTPUT);
#endif
        }
        OLED_I2C_DELAY;
#if !defined OUTPUT_OPEN_DRAIN
        pinMode(scl_pin, INPUT);
#endif
        digitalWrite(scl_pin, HIGH);
        OLED_I2C_DELAY;
        while (!digitalRead(scl_pin));
        digitalWrite(scl_pin, LOW);
#if !defined OUTPUT_OPEN_DRAIN
        pinMode(scl_pin, OUTPUT);
#endif
        OLED_I2C_DELAY;
        byte = byte << 1;
    }
    // Receive ACK
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(sda_pin, INPUT);
#endif
    digitalWrite(sda_pin, HIGH);
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(scl_pin, INPUT);
#endif
    digitalWrite(scl_pin, HIGH);
    OLED_I2C_DELAY;
    while (!digitalRead(scl_pin));
    bool ack = digitalRead(sda_pin) == 0;
    digitalWrite(scl_pin, LOW);
#if !defined OUTPUT_OPEN_DRAIN
    pinMode(scl_pin, OUTPUT);
#endif
    OLED_I2C_DELAY;
    return ack;
}
