#ifndef FS_INTERFACE_H
#define FS_INTERFACE_H
#include <string>
#include <vector>

struct FSFile {
    std::string name;
    std::string content;
};

class FSInterface {
public:
    virtual bool init() = 0;
    virtual bool exists(const std::string& path) = 0;
    virtual bool mkdir(const std::string& path) = 0;
    virtual std::vector<FSFile> listFiles(const std::string& path) = 0;
    virtual FSFile open(const std::string& path) = 0;
    virtual ~FSInterface() = default;
};

#endif
