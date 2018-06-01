#include <Arduino.h>
#include <oled.h>

// There are 2 different versions of the board
// OLED display=OLED(2,14,4); 
OLED display=OLED(4,5,16);

static const uint8_t bitmap[] =
{ 
    1,2,4,8,16,32,64,128,128,128,192,192,128,128,128,64,32,16,8,4,2,1, // first page (8 vertical bits, 22 columns)
    255,255,255,255,15,15,15,15,15,15,15,15,15,15,15,15,15,15,255,255,255,255 // second page (8 vertical bits, 22 columns)
};

void setup()
{
    delay(1000);
    display.begin();   
}


void loop()
{
    // Draw pixels in the outer edges  
    display.draw_pixel(0,0);
    display.draw_pixel(127,0);
    display.draw_pixel(127,31);
    display.draw_pixel(0,31);    
    display.display();
    
    // Draw hollow circles
    for (uint_least8_t radius=3; radius<62; radius+=3)
    {
        delay(50);
        display.draw_circle(64,16,radius);
        if (radius>15)
        {
            display.draw_circle(64,16,radius-15,OLED::SOLID,OLED::BLACK);
        }
        display.display();
    }
    
    // Draw solid circles
    delay(500);
    display.draw_circle(36,16,14,OLED::SOLID);
    display.display();
    delay(500);
    display.draw_circle(36,16,7,OLED::SOLID,OLED::BLACK);
    display.display();
    
    // Draw rectangles
    delay(500);
    display.draw_rectangle(64,0,98,31);
    display.display();
    delay(500);
    display.draw_rectangle(69,5,93,26,OLED::SOLID);
    display.display();

    // scroll up
    delay(1000);
    display.scroll_up(32,20);
    
    // Draw text with normal size
    display.draw_string(4,2,"Hello");
    display.display();
    
    // Draw a line
    delay(1000);
    display.draw_line(4,10,34,10);
    display.display();    
    
    // Draw text from program memory with double size
    delay(1000);
    display.draw_string_P(16,15,PSTR("World!"),OLED::DOUBLE_SIZE);
    display.display();
    
    // Draw a cross
    delay(1000);
    display.draw_line(16,31,88,15);
    display.draw_line(16,15,88,31);
    display.display();
    
    // Draw a raw bitmap
    delay(1000);    
    display.draw_bitmap_P(100,8,22,16,bitmap);
    display.display();
    
    // Demonstrate scrolling
    delay(1000);
    display.set_scrolling(OLED::HORIZONTAL_RIGHT);
    delay(3000);
    display.set_scrolling(OLED::HORIZONTAL_LEFT);
    delay(3000);
    display.set_scrolling(OLED::NO_SCROLLING);
    
    // Redraw after scrolling to get the original picture
    display.display();
    
    // Flash the display
    delay(1000);
    for (int i=0; i<10; i++)
    {
        display.set_invert(true);
        delay(200);
        display.set_invert(false);
        delay(200);
    }
    
    // Show contrast values
    contrast(128);
    contrast(64);
    contrast(32);
    contrast(16);
    contrast(8);
    contrast(4);
    contrast(2);
    contrast(1);
    contrast(2);
    contrast(4);
    contrast(8);
    contrast(16);
    contrast(32);
    contrast(64);
    contrast(128);
    contrast(255);
    contrast(128);
    delay(3000);
          
    display.clear();
}


void contrast(int value)
{
    char buffer[4];
    display.clear();  
    display.draw_string(0,0,"Contrast:");
    itoa(value,buffer,10);
    display.draw_string(64,0,buffer);
    display.draw_rectangle(0,20,value/2,31,OLED::SOLID);
    display.display();
    display.set_contrast(value);
    delay(500);
}