#pragma once

#include "brigadier/argument.hpp"
#include "brigadier/reader/Reader.hpp"

namespace brigadier::parser {
class String : public Argument {
public:
    String() = default;
    ~String() = default;

    virtual void parse(Reader &reader) override;
};
}
