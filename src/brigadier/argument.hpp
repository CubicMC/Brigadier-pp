#pragma once

#include "brigadier/reader/Reader.hpp"
#include <cstdint>
#include <string>

namespace brigadier {
class Argument {
public:
    Argument() = default;

    /**
     * @brief
     *
     * @throw exception-object exception description
     *
     * @param arg
     * @return uint8_t
     */
    virtual void parse(Reader &reader) = 0;
};
}
