#pragma once

#include <brigadier/Parser.hpp>
#include <brigadier/reader/Reader.hpp>
#include <string>

namespace brigadier {
struct StringParser : public Parser {
    using type = std::string;

    static std::string parse(Reader &reader) { return reader.readString(); }
};

struct GreedyStringParser : public Parser {
    using type = std::string;

    static std::string parse(Reader &reader) { return reader.getRemaining(); }
};

} // namespace brigadier
