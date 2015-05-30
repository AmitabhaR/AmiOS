#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"
#include "String.h"

#define GRAPHICS_MODE 0x12
#define TEXT_MODE 0x2

struct COORD
{ 
	int32_t x;
	int32_t y;
};

// Prototypes of some basic functions.
void print(String pstr, int8_t char_color);
void print(String str, int8_t char_col);
void printf(String pstr, int8_t char_color);
void printf(String str, int8_t char_color);
void setCursor(struct COORD pos);
struct COORD getCursor();
void enableMouse();
void disableMouse();
void drawPixel(struct COORD pos, int8_t pix_color);
int8_t getPixel(struct COORD pos);
void clearScreen(byte MODE);

#endif // DISPLAY_H
