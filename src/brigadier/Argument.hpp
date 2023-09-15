#pragma once

#include <brigadier/reader/Reader.hpp>
#include <cstdint>
#include <string>

namespace brigadier {
class Argument {
public:
    Argument(const std::string &name, const std::string &description = "", bool required = false, bool greedy = false):
        _name(name),
        _description(description),
        _required(required),
        _greedy(greedy)
    {
    }
    virtual ~Argument() = default;

    virtual std::string help() const = 0;
    virtual std::string usage() const = 0;

protected:
    const std::string _name;
    const std::string _description;
    const bool _required;
    const bool _greedy;
};
} // namespace brigadier
