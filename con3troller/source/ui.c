#include "ui.h"

int initUI() {
    if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE)) {
        return -1;
    }
    if (!C2D_Init(C2D_DEFAULT_MAX_OBJECTS)) {
        return -2;
    }
    C2D_Prepare();
    return 0;
}

void exitUI() {
    C2D_Fini();
    C3D_Fini();
}

void initContext(DrawContext* ctx)
{
    ctx->top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    ctx->bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

void initColors(DrawContext* ctx)
{
    ctx->clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
    ctx->clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
    ctx->clrRed = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
    ctx->clrBlue = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
    ctx->clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
    ctx->clrYellow = C2D_Color32(0xFF, 0xFF, 0x00, 0xFF);
    ctx->clrBgBright = C2D_Color32(0x25, 0x70, 0xC7, 0xFF);
    ctx->clrBgDark = C2D_Color32(0x10, 0x3C, 0x6F, 0xFF);
}

void drawText(float x, float y, float z, float scale, u32 color, const char* value) //ty mechanicaldragon0687
{
    C2D_TextBuf buf=C2D_TextBufNew(4096);
    C2D_Text text;
    C2D_TextParse(&text, buf, value);
    C2D_TextOptimize(&text);
    C2D_DrawText(&text, C2D_WithColor, x, y, z, scale, scale, color);
    C2D_TextBufDelete(buf);
}