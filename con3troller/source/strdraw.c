#include "strdraw.h"

void getTextLength(float *length, float *height, float scale, char *str) {
    C2D_TextBuf buf=C2D_TextBufNew(4096);
    C2D_Text text;
    C2D_TextParse(&text, buf, str);
    C2D_TextGetDimensions(&text, scale, scale, length, height);
    C2D_TextBufDelete(buf);
}

void drawStringBoxAtPos(float x, float y, float z, float scale, u32 strclr, u32 bgclr, char *str, float margin) {
    float boxwidth;
    float boxheight;
    getTextLength(&boxwidth, &boxheight, scale, str);
    C2D_DrawRectSolid(x-margin, y-margin, z, boxwidth+margin*2, boxheight+margin*2, bgclr);
    drawText(x, y, z, scale, strclr, str);
}

void drawStringBoxXCentered(float y, float z, float scale, u32 strclr, u32 bgclr, char *str, float margin) {
    float boxwidth;
    float boxheight;
    getTextLength(&boxwidth, &boxheight, scale, str);
    float x = SCREEN_WIDTH_TOP/2-(boxwidth/2);
    C2D_DrawRectSolid(x-margin, y-margin, z, boxwidth+margin*2, boxheight+margin*2, bgclr);
    drawText(x, y, z, scale, strclr, str);
}