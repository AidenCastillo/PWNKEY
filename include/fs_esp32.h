#ifdef ESP32
#ifndef FS_ESP32
#define FS_ESP32

#include "fs_interface.h"
#include <SD.h>
#include <SPI.h>

class ESP32FS : public FSInterface {
public:
    bool init() override {
        SPI.begin(SCK, MISO, MOSI, SS);
        return SD.begin(SS, SPI);
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
