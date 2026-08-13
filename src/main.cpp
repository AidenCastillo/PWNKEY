/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#ifdef ESP32
#include <SPI.h>

/*  Install the "TFT_eSPI" library by Bodmer to interface with the TFT Display - https://github.com/Bodmer/TFT_eSPI
    *** IMPORTANT: User_Setup.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE User_Setup.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd/ or https://RandomNerdTutorials.com/esp32-tft/   */
#include <TFT_eSPI.h>

// Install the "XPT2046_Touchscreen" library by Paul Stoffregen to use the Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen
// Note: this library doesn't require further configuration
#include <XPT2046_Touchscreen.h>

#include <debug.h>
#include <wifi_suite.h>
#include <ble_suite.h>
#include <module.h>
#include <fs_esp32.h>
#include <loader.h>
#include "esp_heap_caps.h"
TFT_eSPI tft = TFT_eSPI();            // Physical screen
TFT_eSprite spr = TFT_eSprite(&tft);  // Off-screen buffer

std::vector<Module*> modules = {
    new WiFiModule(),
    new BLEModule()
};

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

// How many modules to show per page
static const int MODULES_PER_PAGE = 5;
static int currentPage = 0;

// Button geometry
static const int BTN_WIDTH  = 100;
static const int BTN_HEIGHT = 40;
static const int BTN_Y      = SCREEN_HEIGHT - 60;

// Forward declaration
void drawModuleScreen(const std::vector<Module*>& modules);

TS_Point getTouch() {
    TS_Point p;

    if (touchscreen.tirqTouched() && touchscreen.touched()) {
        p = touchscreen.getPoint();
        p.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        p.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        return p;
    }

    return TS_Point(-1, -1, -1);
}

void drawButtonSprite(const char* label, int x, int y) {
    spr.fillRoundRect(x, y, BTN_WIDTH, BTN_HEIGHT, 6, TFT_DARKGREY);
    spr.drawRoundRect(x, y, BTN_WIDTH, BTN_HEIGHT, 6, TFT_WHITE);
    spr.drawCentreString(label, x + BTN_WIDTH / 2, y + 10, FONT_SIZE);
}

void drawModuleScreen(const std::vector<Module*>& modules) {
    spr.fillSprite(TFT_WHITE);

    spr.drawCentreString("Available Modules", SCREEN_WIDTH / 2, 20, FONT_SIZE + 2);

    int totalPages = (modules.size() + MODULES_PER_PAGE - 1) / MODULES_PER_PAGE;
    int startIndex = currentPage * MODULES_PER_PAGE;
    int endIndex   = std::min(startIndex + MODULES_PER_PAGE, (int)modules.size());

    int yBase = 80;

    for (int i = startIndex; i < endIndex; i++) {
        int slot = i - startIndex;
        spr.drawCentreString(modules[i]->getName().c_str(),
                             SCREEN_WIDTH / 2,
                             yBase + (slot * 30),
                             FONT_SIZE);
    }

    if (currentPage > 0)
        drawButtonSprite("Prev", 10, BTN_Y);

    if (currentPage < totalPages - 1)
        drawButtonSprite("Next", SCREEN_WIDTH - BTN_WIDTH - 10, BTN_Y);

    char buf[32];
    snprintf(buf, sizeof(buf), "Page %d / %d", currentPage + 1, totalPages);
    spr.drawCentreString(buf, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20, FONT_SIZE);

    // Push final frame to screen
    spr.pushSprite(0, 0);
}

void handleModuleScreenTouch(int x, int y, const std::vector<Module*>& modules) {
    int totalPages = (modules.size() + MODULES_PER_PAGE - 1) / MODULES_PER_PAGE;

    // NEXT
    if (x >= SCREEN_WIDTH - BTN_WIDTH - 10 &&
        x <= SCREEN_WIDTH - 10 &&
        y >= BTN_Y &&
        y <= BTN_Y + BTN_HEIGHT)
    {
        if (currentPage < totalPages - 1) {
            currentPage++;
            drawModuleScreen(modules);
        }
        return;
    }

    // PREV
    if (x >= 10 &&
        x <= 10 + BTN_WIDTH &&
        y >= BTN_Y &&
        y <= BTN_Y + BTN_HEIGHT)
    {
        if (currentPage > 0) {
            currentPage--;
            drawModuleScreen(modules);
        }
        return;
    }
}

Module* detectModuleTap(int x, int y) {
    int startIndex = currentPage * MODULES_PER_PAGE;
    int endIndex   = std::min(startIndex + MODULES_PER_PAGE, (int)modules.size());

    for (int i = startIndex; i < endIndex; i++) {
        int slot = i - startIndex;
        int yStart = 80 + (slot * 30);
        int yEnd   = yStart + 25;

        if (y >= yStart && y <= yEnd) {
            int textW = spr.textWidth(modules[i]->getName().c_str(), FONT_SIZE);
            int xStart = (SCREEN_WIDTH - textW) / 2 - 10;  // 10px padding each side
            int xEnd   = (SCREEN_WIDTH + textW) / 2 + 10;

            if (x >= xStart && x <= xEnd) {
                return modules[i];
            }
        }
    }

    return nullptr;
}

void initDisplay() {
    tft.init();
    tft.setRotation(1);

    spr.setColorDepth(8); // 8-bit color depth

    Serial.printf("Free heap before sprite: %u\n", ESP.getFreeHeap());

    void* result = spr.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (result == nullptr) {
        Serial.println("SPRITE CREATION FAILED - out of memory!");
    }

    Serial.printf("Free heap after sprite: %u\n", ESP.getFreeHeap());
    Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
    Serial.printf("Largest free block: %u\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    Serial.printf("Largest free block (internal): %u\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
}

void initTouchscreen() {
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    pinMode(XPT2046_IRQ, INPUT_PULLUP);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(1);
}

void initSD() {
    fileSystemInterface = new ESP32FS();
    if (fs_init()) {
        loadModules(modules);
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    initDisplay();
    initTouchscreen();
    debug_init();
    initSD();




    spr.fillSprite(TFT_WHITE);
    spr.setTextColor(TFT_BLACK, TFT_WHITE);
    
    spr.drawCentreString("PWNKEY!", SCREEN_WIDTH / 2, 30, FONT_SIZE);
    
    spr.drawCentreString(" /\\_/\\  ", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40, FONT_SIZE);
    spr.drawCentreString("( o.o )", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20, FONT_SIZE);
    spr.drawCentreString("> ^ <", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FONT_SIZE);
    
    spr.drawCentreString("Touch screen to test", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 40, FONT_SIZE);
    spr.pushSprite(0, 0);
    delay(2000);

    drawModuleScreen(modules);
}

void loop() {
    static unsigned long lastTouch = 0;
    unsigned long now = millis();

    if (now - lastTouch > 150) {
        TS_Point p = getTouch();

        if (p.x != -1) {
            lastTouch = now;
            Serial.printf("Touch: x=%d y=%d\n", p.x, p.y);

            handleModuleScreenTouch(p.x, p.y, modules);

            Module* selected = detectModuleTap(p.x, p.y);
            if (selected) {
                Serial.printf("Selected module: %s\n", selected->getName().c_str());
                // Clear screen before module runs
                tft.fillScreen(TFT_WHITE);

                selected->init();
                selected->run();
                selected->cleanup();
                tft.setTextColor(TFT_BLACK, TFT_WHITE);

                // FULL RESET OF SPRITE + MENU
                spr.fillSprite(TFT_WHITE);
                drawModuleScreen(modules);
                spr.pushSprite(0, 0);
            }
        }
    }

    delay(10);
}


extern "C" void app_main() {
    initArduino();
    setup();
    while (true) {
        loop();
        yield();
    }
}

#endif
