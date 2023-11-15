#pragma once

#include "brigadier/reader/StringReader.hpp"
#include <brigadier/CommandNode.hpp>
#include <brigadier/exceptions.hpp>
#include <vector>

namespace brigadier {

class Registry : public ICommandNode {
public:
    Registry &add(const std::shared_ptr<ICommandNode> &node);

    void parse(const std::string &command) const;
    void parse(Reader &reader) const;
    void parse(TypeHolder holder, const std::string &command);

    template<typename T>
    void parse(T &source, Reader &reader) const
    {
        TypeHolder holder(source);
        parse(holder, reader);
    }

    template<typename T>
    void parse(T *source, Reader &reader) const
    {
        TypeHolder holder(source);
        parse(holder, reader);
    }

    /**
     * @brief Parse a command and execute it
     *
     * @param source
     * @param reader
     */
    void parse(TypeHolder &source, Reader &reader) const override;

    constexpr const std::vector<std::shared_ptr<ICommandNode>> &getChildren() const override { return _nodes; }
    constexpr std::string_view getName() const override { return "<root>"; }
    constexpr std::string_view getUsage() const override { return ""; }
    constexpr bool canUse(const TypeHolder &source) override { return true; }
    [[noreturn]] const std::vector<std::string> &getAliases() const override { throw std::runtime_error("Not implemented"); }

    bool isValidInput(Reader &input) const override;
    [[nodiscard]] std::vector<std::string> listSuggestions(TypeHolder &holder, Reader &reader) const override;

private:
    std::vector<std::shared_ptr<ICommandNode>> _nodes;
};

} // namespace brigadier
