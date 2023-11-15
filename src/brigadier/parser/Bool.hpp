#pragma once

#include <brigadier/Parser.hpp>

namespace brigadier {
struct BoolParser : public Parser {
    using type = bool;

    static bool parse(Reader &reader) { return reader.readBool(); }
};

} // namespace brigadier
