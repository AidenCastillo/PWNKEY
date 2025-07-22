#ifndef BLE_SUITE_H
#define BLE_SUITE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <module.h>
#include <Arduino.h>
#include <main.h>

class BLEModule : public Module {
  public:
    String getName() const override {
      return "BLE Module";
    }

    void init() override {
    //   BLEDevice::init("PWNKEY");
      Serial.println("BLE initialized.");
    }

    String getInfo() const override {
      return "BLE Status: ";
    }

    void run() override {
      // BLE operations can be added here
      Serial.println("Running BLE operations...");

      while (true) {
        // Show BLE Screen
        tft.fillScreen(TFT_WHITE);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
        tft.drawCentreString("BLE Connected", SCREEN_WIDTH / 2, 30, FONT_SIZE);
        tft.drawCentreString("Scanning for devices...", SCREEN_WIDTH / 2, 80
                                , FONT_SIZE);
        delay(2000); // Simulate scanning delay

      }
    }

    void cleanup() override {
    //   BLEDevice::deinit();
      Serial.println("BLE deinitialized.");
    }
};

#endif
