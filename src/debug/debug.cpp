#include <debug.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <main.h>

char debugLines[25][50];  // Array to hold 25 debug lines, each up to 50 characters

void debug_init() {
    // Initialize a array to hold debug lines
    // Clear the debug lines
    memset(debugLines, 0, sizeof(debugLines));

    Serial.println("Debug initialized.");
}

void debug_to_screen(const char *message) {
    // Print the debug message to the TFT display
    // The messages will scroll up the screen as new messages are added
    // New messages are added to the end of the debugLines array
    // Shift messages down to make room for the new message

    for (int i = 24; i > 0; i--) {
        strncpy(debugLines[i], debugLines[i - 1], sizeof(debugLines[i]) - 1);
        debugLines[i][sizeof(debugLines[i]) - 1] = '\0'; // Ensure null termination
    }

    strncpy(debugLines[0], message, sizeof(debugLines[0]) - 1);
    debugLines[0][sizeof(debugLines[0]) - 1] = '\0'; // Ensure null termination
    // Clear the TFT screen
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    // Print the debug lines on the TFT display
    for (int i = 0; i < 25; i++) {
        if (debugLines[i][0] != '\0') { // Only print non-empty lines
            tft.drawString(debugLines[i], 10, 10 + (i * 20), 2);
        }
    }
    // Also print to Serial Monitor
    Serial.println(message);
}

