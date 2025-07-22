#include "wifi_suite.h"
#include <WiFi.h>
// #include <module.h>

// class WiFiModule : public Module {
//   public:
//     String getName() const override {
//       return "WiFi Module";
//     }

//     void init() override {
//       WiFi.mode(WIFI_STA);
//       WiFi.begin("your-ssid", "your-password");
//     }

//     String getInfo() const override {
//       return "WiFi Status: " + String(WiFi.status());
//     }

//     void run() override {
//       if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("WiFi connected");
//       } else {
//         Serial.println("WiFi not connected");
//       }
//     }

//     void cleanup() override {
//       WiFi.disconnect();
//     }
// };
