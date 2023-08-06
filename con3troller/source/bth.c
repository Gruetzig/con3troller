#include "bth.h"

void newButton(Button *button, float x, float y, float z, float width, float height, char* text, float corners, u32 strclr, u32 bgclr, float maxscale) {
    button->tbuf = C2D_TextBufNew(4096);
    C2D_TextParse(&(button->text), button->tbuf, text);
    C2D_TextOptimize(&(button->text));
    button->x = x;
    button->y = y;
    button->z = z;
    button->corners = corners;
    button->width = width;
    button->height = height;
    button->strclr = strclr;
    button->bgclr = bgclr;
    float textwidth;
    float textheight;
    C2D_TextGetDimensions(&(button->text), maxscale, maxscale, &textwidth, &textheight);
    button->tscale = maxscale;
    button->textlength = textwidth;
    button->textheight = textheight;
}

void drawButton(Button *button) {
    C2D_DrawRectSolid((button->x) + (button->corners), (button->y) + (button->corners), (button->z), (button->width) - (button->corners)*2, (button->height) - (button->corners)*2, (button->bgclr));
    float strplacementx = button->x + (button->width - button->textlength) / 2;
    float strplacementy = button->y + (button->height - button->textheight) / 2;
    C2D_DrawText(&(button->text), C2D_WithColor, strplacementx, strplacementy, button->z, button->tscale, button->tscale, button->strclr);
}

bool isButtonPressed(Button *button) {
    touchPosition touch;
    hidTouchRead(&touch);
    u32 kDown = hidKeysDown();
    if (kDown & KEY_TOUCH) {
        if (((touch.px <= button->x + button->width) && (touch.px >= button->x)) && ((touch.py <= button->y + button->height) && (touch.py >= button->y))) {
            return true;
        }
    }
    return false;
}

void destroyButton(Button *button) {
    C2D_TextBufDelete(button->tbuf);
}

void changeButtonString(Button *button, char* string) {
    C2D_TextBufClear(button->tbuf);
    C2D_TextParse(&(button->text), button->tbuf, string);
    C2D_TextOptimize(&(button->text));
    float textwidth;
    float textheight;
    C2D_TextGetDimensions(&(button->text), button->tscale, button->tscale, &textwidth, &textheight);
    button->textlength = textwidth;
    button->textheight = textheight;
}