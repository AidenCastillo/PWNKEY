#ifndef MODULE_H
#define MODULE_H

#include <WString.h>

class Module {
  public:
    virtual String getName() const = 0;
    virtual void init() = 0;
    virtual String getInfo() const = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;
};

#endif // MODULE_H
