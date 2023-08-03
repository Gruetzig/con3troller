#ifndef CON3TROLLER_UI_H
#define CON3TROLLER_UI_H

#include <citro2d.h>
#include <3ds.h>

#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOTTOM 320
#define SCREEN_HEIGHT 240

typedef struct {
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    u32 clrBlack;
    u32 clrWhite;
    u32 clrRed;
    u32 clrBlue;
    u32 clrGreen;
    u32 clrYellow;
    u32 clrBgBright;
    u32 clrBgDark;
} DrawContext;

int initUI();
void exitUI();
void initContext(DrawContext* ctx);
void initColors(DrawContext* ctx);
void drawText(float x, float y, float z, float scale, u32 color, const char* value);

#endif