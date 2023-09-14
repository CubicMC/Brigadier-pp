#pragma ocne

#include <concepts>
#include <type_traits>
#include <string>

namespace brigadier {
/**
 * This class is only here to store the difference parser
 * For your class to be a parser, it must inherit from this class and must comply with the conditions of the @see is_parser concept
 */
class Parser { };

/**
 * @brief This concept is used to check if a class is a parser
 *
 * @tparam T The class to check
 */
template<typename T>
concept is_parser = std::is_base_of_v<Parser, T> && requires(T t) {
    typename T::type;
    // clang-format off
    { t.parse() } -> std::same_as<typename T::type>;
    { t.help() } -> std::same_as<std::string>;
    { t.usage() } -> std::same_as<std::string>;
    // clang-format on
};

} // namespace brigadier
