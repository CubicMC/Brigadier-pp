#pragma once

#include <brigadier/reader/Reader.hpp>
#include <cstddef>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#define POPULATE_BASIC_EXCEPTION(name, exception) \
public:                                           \
    name(char const *const message) throw():      \
        exception(message)                        \
    {                                             \
    }                                             \
    name(const std::string &message) throw():     \
        exception(message)                        \
    {                                             \
    }

#define DEFINE_EXCEPTION_FROM(name, exception)    \
    class name : public exception {               \
        POPULATE_BASIC_EXCEPTION(name, exception) \
    }
#define DEFINE_EXCEPTION_FROM_C(name, exception, arguments, stringBuilder) \
    class name : public exception {                                        \
        POPULATE_BASIC_EXCEPTION(name, exception)                          \
        name arguments throw():                                            \
            exception([&, this ]() stringBuilder())                        \
        {                                                                  \
        }                                                                  \
    }

#define DEFINE_EXCEPTION(name) DEFINE_EXCEPTION_FROM(name, std::runtime_error)
#define DEFINE_EXCEPTION_C(name, arguments, stringBuilder) DEFINE_EXCEPTION_FROM_C(name, std::runtime_error, arguments, stringBuilder)

namespace brigadier {
DEFINE_EXCEPTION(DispatcherException);

//* Reader
DEFINE_EXCEPTION(ReaderException);

DEFINE_EXCEPTION_FROM_C(CommandSyntaxException, ReaderException, (const std::string &message, Reader &reader), {
    return fmt::format("{} at position {}", message, reader.getCursor());
});

//* Parser
DEFINE_EXCEPTION(ParserException);

DEFINE_EXCEPTION_FROM(ArgumentException, ParserException);

//* TypeHolder
DEFINE_EXCEPTION(TypeHolderException);

DEFINE_EXCEPTION_FROM(InvalidTypeException, TypeHolderException);

} // namespace brigadier
