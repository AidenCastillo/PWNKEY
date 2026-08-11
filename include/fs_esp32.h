#ifdef ESP32
#ifndef FS_ESP32
#define FS_ESP32

#include "fs_interface.h"
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN 5    // SD card Chip Select - Use a pin that doesn't conflict with touchscreen
#define SD_SCK_PIN 18   // SD card clock
#define SD_MISO_PIN 19  // SD card MISO
#define SD_MOSI_PIN 23  // SD card MOSI

// // Create a dedicated SPI instance for the SD card to avoid conflicts
SPIClass sdSPI = SPIClass(HSPI); // Use HSPI for SD card (different from touchscreen's VSPI)
class ESP32FS : public FSInterface {
public:
    bool init() override {
        Serial.println("Initializing SD card...");
        
        sdSPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
        // Initialize SD card
        if (!SD.begin(SD_CS_PIN, sdSPI)) {
            Serial.println("ERROR: Card Mount Failed");
            Serial.println("       Make sure the SD card is properly inserted and wired correctly");
            return false;
        }
        
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE) {
            Serial.println("ERROR: No SD card attached");
            return false;
        }
        
        Serial.print("SD Card Type: ");
        if (cardType == CARD_MMC) {
            Serial.println("MMC");
        } else if (cardType == CARD_SD) {
            Serial.println("SDSC");
        } else if (cardType == CARD_SDHC) {
            Serial.println("SDHC");
        // } else if (cardType == CARD_SDXC) {
        //   Serial.println("SDXC (exFAT)");
        } else {
            Serial.println("UNKNOWN");
        }
        
        // Print SD card details
        uint64_t cardSize = SD.cardSize() / (1024 * 1024);
        uint64_t totalBytes = SD.totalBytes() / 1024;
        uint64_t usedBytes = SD.usedBytes() / 1024;
        uint64_t freeBytes = totalBytes - usedBytes;
        
        Serial.printf("SD Card Size: %lluMB\n", cardSize);
        Serial.printf("SD Card Info: Total: %lluKB, Used: %lluKB, Free: %lluKB\n", 
                        totalBytes, usedBytes, freeBytes);
        return SD.begin(SD_CS_PIN, sdSPI);
    }

    bool exists(const std::string& path) override {
        return SD.exists(path.c_str());
    }

    bool mkdir(const std::string& path) override {
        return SD.mkdir(path.c_str());
    }

    std::vector<FSFile> listFiles(const std::string& path) override {
        std::vector<FSFile> files;
        File root = SD.open(path.c_str());
        File file = root.openNextFile();
        while (file) {
            if (!file.isDirectory()) {
                FSFile f;
                f.name = file.name();
                f.content = file.readString().c_str();
                files.push_back(f);
            }
            file = root.openNextFile();
        }
        return files;
    }

    FSFile open(const std::string& path) override {
        File f = SD.open(path.c_str());
        FSFile out;
        out.name = path;
        out.content = f.readString().c_str();
        return out;
    }
};

#endif
#endif
