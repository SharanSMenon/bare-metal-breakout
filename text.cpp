#include "screenfont.h"
#include "text.h"

// Move these functions to a separate file.
void drawChar(C2DGraphics *p_graphics, unsigned char ch, int x, int y, TScreenColor col, int zoom)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;
    for (int i = 1; i <= (FONT_HEIGHT * zoom); i++)
    {
        for (int j = 0; j < (FONT_WIDTH * zoom); j++)
        {
            unsigned char mask = 1 << (j / zoom);
            if (*glyph & mask)
            {
                p_graphics->DrawPixel(x + j, y + i, col);
            }
        }
        glyph += (i % zoom) ? 0 : FONT_BPL;
    }
}

void drawString(C2DGraphics *p_graphics, int x, int y, char *s, TScreenColor col, int zoom)
{
    while (*s)
    {
        if (*s == '\r')
        {
            x = 0;
        }
        else if (*s == '\n')
        {
            x = 0;
            y += (FONT_HEIGHT * zoom);
        }
        else
        {
            drawChar(p_graphics, *s, x, y, col, zoom);
            x += (FONT_WIDTH * zoom);
        }
        s++;
    }
}
