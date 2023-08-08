#include "utils.h"

size_t getComboString(u32 bbt, char* out) {
    strcpy(out, "");
    if (bbt & KEY_A) {
        strcat(out, "A+");
    }
    if (bbt & KEY_B) {
        strcat(out, "B+");
    }
    if (bbt & KEY_X) {
        strcat(out, "X+");
    }
    if (bbt & KEY_Y) {
        strcat(out, "Y+");
    }
    if (bbt & KEY_START) {
        strcat(out, "START+");
    }
    if (bbt & KEY_SELECT) {
        strcat(out, "SELECT+");
    }
    if (bbt & KEY_DUP) {
        strcat(out, "DPad Up+");
    }
    if (bbt & KEY_DDOWN) {
        strcat(out, "DPad Down+");
    }
    if (bbt & KEY_DLEFT) {
        strcat(out, "DPad Left+");
    }
    if (bbt & KEY_DRIGHT) {
        strcat(out, "DPad Right+");
    }
    if (bbt & KEY_L) {
        strcat(out, "L+");
    }
    if (bbt & KEY_R) {
        strcat(out, "R+");
    }
    if (bbt & KEY_ZL) {
        strcat(out, "ZL+");
    }
    if (bbt & KEY_ZR) {
        strcat(out, "ZR+");
    }
    if (bbt & KEY_CPAD_UP) {
        strcat(out, "CPad Up+");
    }
    if (bbt & KEY_CPAD_DOWN) {
        strcat(out, "CPad Down+");
    }
    if (bbt & KEY_CPAD_LEFT) {
        strcat(out, "CPad Left+");
    }
    if (bbt & KEY_CPAD_RIGHT) {
        strcat(out, "CPad Right+");
    }
    if (bbt & KEY_CSTICK_UP) {
        strcat(out, "CStick Up+");
    }
    if (bbt & KEY_CSTICK_DOWN) {
        strcat(out, "CStick Down+");
    }
    if (bbt & KEY_CSTICK_LEFT) {
        strcat(out, "CStick Left+");
    }
    if (bbt & KEY_CSTICK_RIGHT) {
        strcat(out, "CStick Right+");
    }
    if (bbt & KEY_TOUCH) {
        strcat(out, "Touchscreen+");
    }
    size_t outlen = strlen(out);
    out[outlen] = '\0';
    return outlen;
}