#pragma once

#include <string_view>

namespace brigadier {

struct Argument {
    const std::string_view name;
    const std::string_view description;
    const bool required;
};

} // namespace brigadier
