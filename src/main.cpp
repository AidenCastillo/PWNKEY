/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

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
#include <loader.h>

// Global TFT display object for all files
TFT_eSPI tft = TFT_eSPI();
Module *modules[] = {
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

// Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}

// Print Touchscreen info about X, Y and Pressure (Z) on the TFT Display
void printTouchToDisplay(int touchX, int touchY, int touchZ) {
  // Clear TFT screen
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  int centerX = SCREEN_WIDTH / 2;
  int textY = 80;
 
  String tempText = "X = " + String(touchX);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Y = " + String(touchY);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

  textY += 20;
  tempText = "Pressure = " + String(touchZ);
  tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n===== PWNKEY Starting =====");

  // Initialize TFT display first
  Serial.println("Initializing TFT display...");
  tft.init();
  tft.setRotation(1); // Set landscape mode
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  Serial.println("TFT initialized");

  // Start the dedicated SPI for the touchscreen 
  Serial.println("Initializing touchscreen SPI...");
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  
  pinMode(XPT2046_IRQ, INPUT_PULLUP);
  
  // Initialize the touchscreen
  Serial.println("Starting touchscreen...");
  if (touchscreen.begin(touchscreenSPI)) {
    Serial.println("Touchscreen initialized successfully");
  } else {
    Serial.println("WARNING: Touchscreen initialization may have failed");
  }

  touchscreen.setRotation(1);
  
  Serial.println("Touchscreen configured");

  // Set X and Y coordinates for center of display
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  
  // Display startup message
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("PWNKEY Starting...", centerX, centerY - 20, FONT_SIZE);
  tft.drawCentreString("Initializing hardware", centerX, centerY, FONT_SIZE);

  // Begin PWNKEY initialization
  Serial.println("Initializing debug system...");
  debug_init();
  
  delay(500); // Give a small delay before initializing SD
  
  Serial.println("\n=== Starting SD Card Initialization ===");
  tft.drawCentreString("Initializing SD card...", centerX, centerY + 20, FONT_SIZE);
  
  bool sd_success = fs_init();
  if (sd_success) {
    tft.drawCentreString("SD card ready", centerX, centerY + 40, FONT_SIZE);
    Serial.println("SD card initialization successful");
    
    // Only load modules if SD card initialized successfully
    Serial.println("Loading modules...");
    loadModules();
  } else {
    tft.drawCentreString("SD card failed!", centerX, centerY + 40, FONT_SIZE);
    Serial.println("SD card initialization failed!");
  }
  
  // Allow time to see the SD card status
  delay(1500);
  
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
  tft.drawCentreString("PWNKEY!", centerX, 30, FONT_SIZE);
  
  tft.drawCentreString(" /\\_/\\  ", centerX, centerY - 40, FONT_SIZE);
  tft.drawCentreString("( o.o )", centerX, centerY - 20, FONT_SIZE);
  tft.drawCentreString("> ^ <", centerX, centerY, FONT_SIZE);
  
  tft.drawCentreString("Touch screen to test", centerX, centerY + 40, FONT_SIZE);
  delay(2000);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("PWNKEY Menu", SCREEN_WIDTH / 2, 30, FONT_SIZE);
  tft.drawCentreString("1. WiFi Suite", SCREEN_WIDTH / 2, 80, FONT_SIZE);
  tft.drawCentreString("2. BLE Suite", SCREEN_WIDTH / 2, 110, FONT_SIZE);
}

TS_Point screenTouched() {
  TS_Point p;
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    p.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    p.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    p.z = p.z; // Pressure
    // Print Touchscreen info on Serial Monitor
    printTouchToSerial(p.x, p.y, p.z);
  } else {
    p.x = -1; // No touch
    p.y = -1; // No touch
    p.z = -1; // No pressure
  }
  delay(100); // Debounce delay
  return p;
}

Module* selectModule(int x, int y) {
  // Check if the touch coordinates match a module's area
  if (y > 70 && y < 90) {
    // WiFi Suite selected
    Serial.println("WiFi Suite selected");
    for (Module* module : modules) {
      if (module->getName() == "WiFi Module") {
        module->init();
        module->run();
        return module;
      }
    }
  } else if (y > 100 && y < 120) {
    // BLE Suite selected
    Serial.println("BLE Suite selected");
    for (Module* module : modules) {
      if (module->getName() == "BLE Module") {
        module->init();
        module->run();
        return module;
      }
    }
  }
  return nullptr; // Return nullptr if no selection was made
}

void loop() {
  static unsigned long lastTouchTime = 0;
  unsigned long currentTime = millis();
  
  // Only process touches if enough time has passed since the last valid touch
  if (currentTime - lastTouchTime > 500) {
    TS_Point p = screenTouched();
    
    if (p.x != -1 && p.y != -1) {
      lastTouchTime = currentTime;
      
      // Visual feedback for touch
      tft.fillCircle(p.x, p.y, 5, TFT_RED);
      delay(100);
      Module* module = selectModule(p.x, p.y);
      if (module) {
        // If a module was selected, run its cleanup method when done
        Serial.println("Running module: " + module->getName());
        module->run();
        module->cleanup();
        
        // After module execution, redraw the menu
        tft.fillScreen(TFT_WHITE);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.drawCentreString("PWNKEY Menu", SCREEN_WIDTH / 2, 30, FONT_SIZE);
        tft.drawCentreString("1. WiFi Suite", SCREEN_WIDTH / 2, 80, FONT_SIZE);
        tft.drawCentreString("2. BLE Suite", SCREEN_WIDTH / 2, 110, FONT_SIZE);
      }
    }
  }
  
  // Short delay to prevent CPU hogging
  delay(10);
}

  // if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // // Get Touchscreen points
    // TS_Point p = touchscreen.getPoint();
    // // Calibrate Touchscreen points with map function to the correct width and height
    // x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    // y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    // z = p.z;

    // printTouchToSerial(x, y, z);
    // printTouchToDisplay(x, y, z);

    // delay(100);
    // Load PWNKEY menu
    // tft.fillScreen(TFT_WHITE);
    // tft.setTextColor(TFT_BLACK, TFT_WHITE);
    // tft.drawCentreString("PWNKEY Menu", SCREEN_WIDTH / 2, 30, FONT_SIZE);
    // tft.drawCentreString("1. WiFi Suite", SCREEN_WIDTH / 2, 80, FONT_SIZE);
    // tft.drawCentreString("2. BLE Suite", SCREEN_WIDTH / 2, 110, FONT_SIZE);
    
    // // Wait for touch input to select an option
    // TS_Point p = touchscreen.getPoint();
    // x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    // y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    // z = p.z;

    // printTouchToSerial(x, y, z);
    // // printTouchToDisplay(x, y, z);

    // // Check which menu option was selected
    // if (y > 70 && y < 90) {
    //   // WiFi Suite selected
    //   Serial.println("WiFi Suite selected");
    //   // loadWiFiMenu();
    //   for (Module* module : modules) {
    //     if (module->getName() == "WiFi Module") {
    //       module->init();
    //       module->run();
    //       break;
    //     }
    //   }
    // } else if (y > 100 && y < 120) {
    //   // BLE Suite selected
    //   Serial.println("BLE Suite selected");
    //   // loadBLEMenu();
    //   for (Module* module : modules) {
    //     if (module->getName() == "BLE Module") {
    //       module->init();
    //       module->run();
    //       break;
    //     }
    //   }
    // }
  // }

// Add at the end of your file to satisfy ESP-IDF requirements
extern "C" void app_main() {
  initArduino();
  setup();
  while(true) {
    loop();
    yield();
  }
}
