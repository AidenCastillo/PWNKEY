#ifndef MODULE_H
#define MODULE_H

#include <string>

class Module {
  public:
    virtual std::string getName() const = 0;
    virtual void init() = 0;
    virtual std::string getInfo() const = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;
};

#endif // MODULE_H
