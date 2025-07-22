#ifndef WIFI_SUITE_H
#define WIFI_SUITE_H
#include <Arduino.h>
#include <WiFi.h>
#include <module.h>
#include <main.h>

class WiFiModule : public Module {
  public:
    String getName() const override {
        return "WiFi Module";
    }

    void init() override {
        // WiFi.mode(WIFI_STA);
        WiFi.begin("network", "password");
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("WiFi connected");
        }
    }

    String getInfo() const override {
        return "WiFi Status: " + String(WiFi.status());
    }

    void run() override {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("WiFi connected");
        } else {
            Serial.println("WiFi not connected");
            return;
        }
        // Loop until user wants to exit

        while (true) {
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
    
            // update screen with WiFi info
            tft.fillScreen(TFT_WHITE);
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
            tft.drawCentreString("WiFi Connected", SCREEN_WIDTH / 2, 30, FONT_SIZE);
            tft.drawCentreString("IP: " + WiFi.localIP().toString(),
                                    SCREEN_WIDTH / 2, 80, FONT_SIZE);
        }

        
    }

    void cleanup() override {
        WiFi.disconnect();
    }
};

#endif
