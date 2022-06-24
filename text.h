#ifndef _text_h
#define _text_h

#include <circle/2dgraphics.h>

void drawChar(C2DGraphics *p_graphics, unsigned char ch, int x, int y, TScreenColor col, int zoom);
void drawString(C2DGraphics *p_graphics, int x, int y, char *s, TScreenColor col, int zoom);

#endif