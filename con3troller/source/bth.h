/**
 * 3DS button handler with citro2d and libctru
 * @brief Draws buttons and checks if theyre pressed
 * @author Gruetzig
*/

/* libctru and citro2d includes */
#include <3ds.h>
#include <citro2d.h>

/* standard library includes */
#include <stdbool.h>

/* button struct */

typedef struct buttonstruct {
    float x;
    float y;
    float z;
    float width;
    float height;
    float tscale;
    C2D_Text text;
    C2D_TextBuf tbuf;
    float corners; 
    u32 strclr;
    u32 bgclr;
    float textheight;
    float textlength;
} Button;

/* functions */

void newButton(Button *button, float x, float y, float z, float width, float height, char* text, float corners, u32 strclr, u32 bgclr, float maxscale);
void drawButton(Button *button);
bool isButtonPressed(Button *button);
void destroyButton(Button *button);
void changeButtonString(Button *button, char* string);
