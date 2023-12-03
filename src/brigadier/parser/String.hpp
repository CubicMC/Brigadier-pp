#pragma once

#include "brigadier/exceptions.hpp"
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

    static std::string parse(Reader &reader)
    {
        auto str = reader.getRemaining();
        if (str.empty())
            throw CommandSyntaxException("Expected string", reader);
        reader.setCursor(reader.getTotalLength());
        return str;
    }
};

} // namespace brigadier
