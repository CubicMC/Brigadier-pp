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

DEFINE_EXCEPTION_FROM(CommandSyntaxException, ReaderException);

// class ExpectedValue : public ReaderException {
//     POPULATE_BASIC_EXCEPTION(ExpectedValue, ReaderException)
// public:
//     ExpectedValue(Reader *reader, const std::string &expected) throw():
//         ReaderException("")
//     {
//     }

//     std::string getContext() const
//     {

//     }

//     const char *what() const throw()
//     {
//         // return fmt::format("{}\n{}", ReaderException::what(), getContext()).c_str();
//     }
// };

// constexpr size_t MAX_CONTEXT = 10;
// constexpr inline std::string getExpectedValueMessage(Reader *reader, const std::string &expected)
// {
//     auto cursor = reader->getCursor();
//     if (cursor > MAX_CONTEXT)
//         return fmt::format("Expected {}\n...{}<--[HERE]", expected, reader->getString().substr(std::max(0ul, cursor - MAX_CONTEXT), cursor));
//     return fmt::format("Expected {}\n{}<--[HERE]", expected, reader->getRead());
// };

// DEFINE_EXCEPTION_FROM_C(ExpectedBool, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "a bool");
// });

// DEFINE_EXCEPTION_FROM_C(ExpectedInt, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "an int");
// });

// DEFINE_EXCEPTION_FROM_C(ExpectedLong, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "a long");
// });

// DEFINE_EXCEPTION_FROM_C(ExpectedDouble, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "a double");
// });

// DEFINE_EXCEPTION_FROM_C(ExpectedFloat, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "a float");
// });

// DEFINE_EXCEPTION_FROM_C(ExpectedString, ExpectedValue, (Reader *reader), {
//     return getExpectedValueMessage(reader, "a string");
// });

// DEFINE_EXCEPTION_FROM(InvalidValue, ReaderException);
// DEFINE_EXCEPTION_FROM_C(InvalidBool, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid bool '{}'", value);}
// );

// DEFINE_EXCEPTION_FROM_C(InvalidInt, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid int '{}'", value);}
// );

// DEFINE_EXCEPTION_FROM_C(InvalidLong, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid long '{}'", value);}
// );

// DEFINE_EXCEPTION_FROM_C(InvalidDouble, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid double '{}'", value);}
// );

// DEFINE_EXCEPTION_FROM_C(InvalidFloat, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid float '{}'", value);}
// );

// DEFINE_EXCEPTION_FROM_C(InvalidString, InvalidValue, (Reader *reader, const std::string &value), {
//     return fmt::format("Invalid string '{}'", value);}
// );

} // namespace brigadier
