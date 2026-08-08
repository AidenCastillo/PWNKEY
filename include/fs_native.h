#ifdef NATIVE
#ifndef FS_NATIVE
#define FS_NATIVE
#include "fs_interface.h"
#include <filesystem>
#include <fstream>

class NativeFS : public FSInterface {
public:
    bool init() override {
        return true; // nothing to init
    }

    bool exists(const std::string& path) override {
        return std::filesystem::exists(path);
    }

    bool mkdir(const std::string& path) override {
        return std::filesystem::create_directories(path);
    }

    std::vector<FSFile> listFiles(const std::string& path) override {
        std::vector<FSFile> files;
        for (auto& entry : std::filesystem::directory_iterator(path)) {
            if (!entry.is_directory()) {
                FSFile f;
                f.name = entry.path().string();
                std::ifstream in(entry.path());
                f.content.assign(std::istreambuf_iterator<char>(in), {});
                files.push_back(f);
            }
        }
        return files;
    }

    FSFile open(const std::string& path) override {
        FSFile f;
        f.name = path;
        std::ifstream in(path);
        f.content.assign(std::istreambuf_iterator<char>(in), {});
        return f;
    }
};

#endif
#endif
