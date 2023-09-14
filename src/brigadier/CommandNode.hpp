#pragma once

#include <string>

namespace brigadier {

class CommandNode {
public:
    CommandNode(const std::string &name):
        _name(name)
    {
    }
    virtual ~CommandNode() = default;

    virtual void help() = 0;
    // virtual void usage() = 0;

protected:
    const std::string _name;
};
}
