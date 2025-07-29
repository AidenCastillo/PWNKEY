#ifndef LOADER_H
#define LOADER_H

#include <Arduino.h>
#include <FS.h>
// #include <SPIFFS.h>
#include <SD.h>
#include <SPI.h>
#include <main.h>
#include <module.h>

// Make File class available
using fs::File;

// SPI configuration for SD card
// IMPORTANT: Make sure these pins don't conflict with touchscreen pins
#define SD_CS_PIN 5    // SD card Chip Select - Use a pin that doesn't conflict with touchscreen
#define SD_SCK_PIN 18   // SD card clock
#define SD_MISO_PIN 19  // SD card MISO
#define SD_MOSI_PIN 23  // SD card MOSI

// Create a dedicated SPI instance for the SD card to avoid conflicts
SPIClass sdSPI = SPIClass(HSPI); // Use HSPI for SD card (different from touchscreen's VSPI)

typedef struct {
    String name;
    Module* module;
} ModuleEntry;


bool fs_init() {
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
                
  if (!SD.exists("/scripts")) {
    Serial.println("Creating /scripts directory...");
    if (SD.mkdir("/scripts")) {
      Serial.println("/scripts directory created");
      
      File f = SD.open("/scripts/.placeholder", FILE_WRITE);
      if (f) {
        f.println("# This file is a placeholder in the scripts directory");
        f.close();
      }
    } else {
      Serial.println("Failed to create /scripts directory");
    }
  }
  
  return true;
}

void loadModules() {
  Serial.println("Loading modules...");
  
  // Load module files from SD card in /scripts directory
  if (!SD.exists("/scripts")) {
    Serial.println("ERROR: Scripts directory not found. Call fs_init() before loadModules()");
    return;
  }
  
  File root = SD.open("/scripts");
  if (!root) {
    Serial.println("Failed to open scripts directory");
    return;
  }
  
  if (!root.isDirectory()) {
    Serial.println("/scripts is not a directory");
    root.close();
    return;
  }
  
  File file = root.openNextFile();
  int moduleCount = 0;
  
  while (file) {
    String fileName = file.name();
    
    // Skip placeholder files
    if (fileName.endsWith("/.placeholder") || fileName == "/.placeholder") {
      Serial.println("Skipping placeholder file");
      file = root.openNextFile();
      continue;
    }
    
    if (!file.isDirectory()) {
      // Print full path info for debugging
      Serial.printf("Found file: %s, Size: %d bytes\n", file.name(), file.size());
      
      // Extract just the filename without the path
      String baseName = fileName;
      int lastSlash = fileName.lastIndexOf('/');
      if (lastSlash >= 0) {
        baseName = fileName.substring(lastSlash + 1);
      }
      
      Serial.printf("Loading module: %s\n", baseName.c_str());
      moduleCount++;

      Serial.printf("file content: %s\n", file.readStringUntil('\n').c_str());
      
      // Load the module
      // loadModule(file);
    } else {
      Serial.printf("Found directory: %s (skipping)\n", file.name());
    }
    file = root.openNextFile();
  }
  
  Serial.printf("Module loading complete. Found %d modules.\n", moduleCount);
  root.close();
}

#endif
