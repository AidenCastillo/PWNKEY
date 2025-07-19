#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

extern char debugLines[25][50];  // Array to hold 25 debug lines, each up to 50 characters

void debug_init();

void debug_to_screen(const char *message);

#endif
