#pragma once

#include <brigadier/Parser.hpp>
#include <limits>

namespace brigadier {
template<typename T>
struct NumberParser : public Parser {
    NumberParser() = delete;
};

template<>
struct NumberParser<int> : public Parser {
    using type = int;

    static int parse(Reader &reader) { return reader.readInt(); }
};

template<>
struct NumberParser<long> : public Parser {
    using type = long;

    static long parse(Reader &reader) { return reader.readLong(); }
};

template<>
struct NumberParser<float> : public Parser {
    using type = float;

    static float parse(Reader &reader) { return reader.readFloat(); }
};

template<>
struct NumberParser<double> : public Parser {
    using type = double;

    static double parse(Reader &reader) { return reader.readDouble(); }
};

} // namespace brigadier
