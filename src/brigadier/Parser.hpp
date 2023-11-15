#pragma once

#include <brigadier/reader/Reader.hpp>
#include <concepts>
#include <string>
#include <type_traits>

namespace brigadier {

/**
 * @brief A parser of objects of type T
 *
 * This class is only here to store the difference parser
 * For your class to be a parser, it must inherit from this class and must comply with the conditions of the @see is_parser concept
 * Basically, it must have a static method named parse that takes a Reader as parameter and returns an object of type T
 * example:
 * @code
 * struct MyParser : public Parser {
 *    using type = int;
 *
 *   static int parse(Reader &reader) { return reader.readInt(); }
 * };
 * @endcode
 */
struct Parser {
    Parser() = delete;
};

/**
 * @brief Check if type T is a brigadier parser
 *
 * @tparam T The class to check
 */
template<typename T>
concept is_parser = std::is_base_of<Parser, T>::value && std::is_base_of<std::false_type, std::is_same<typename T::type, void>>::value && requires(Reader &reader) {
    // clang-format off
    { T::parse(reader) } -> std::same_as<typename T::type>;
    // clang-format on
};

} // namespace brigadier
