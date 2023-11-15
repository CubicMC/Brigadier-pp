#pragma once

#include <string>
#include <vector>

#include <brigadier/TypeHolder.hpp>
#include <brigadier/reader/Reader.hpp>

namespace brigadier {
class ICommandNode {
public:
    virtual ~ICommandNode() = default;

    virtual void parse(TypeHolder &source, Reader &reader) const = 0;
    virtual const std::vector<std::shared_ptr<ICommandNode>> &getChildren() const = 0;
    virtual std::string_view getName() const = 0;
    virtual std::string_view getUsage() const = 0;
    virtual bool canUse(const TypeHolder &source) = 0;
    virtual bool isValidInput(Reader &input) const = 0;
    virtual std::vector<std::string> listSuggestions(TypeHolder &holder, Reader &reader) const = 0;
    virtual const std::vector<std::string> &getAliases() const = 0;

    // virtual void findAmbiguities(std::shared_ptr<ICommandNode> parent, AmbiguityConsumer &consumer) = 0;
};
} // namespace brigadier
