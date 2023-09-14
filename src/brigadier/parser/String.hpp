#pragma once

#include <brigadier/reader/Reader.hpp>
#include <brigadier/Parser.hpp>
#include <string>

namespace brigadier {
class String : public Parser {
public:
    String() = default;
    ~String() = default;

    std::string parse(Reader &reader);
};

} // namespace brigadier
