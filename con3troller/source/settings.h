#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int parseIP(char* out);
bool validIP(const char* ip);
void selectIP(char *inout);
void saveIP(char *in);
bool allowHID();
bool allowTouch();
void toggleHID();
void toggleTouch();
