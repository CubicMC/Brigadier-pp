#pragma once

#include <bits/utility.h>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>

#include <brigadier/Argument.hpp>
#include <brigadier/ICommandNode.hpp>
#include <brigadier/Parser.hpp>
#include <brigadier/TypeHolder.hpp>
#include <brigadier/exceptions.hpp>
#include <brigadier/utils.hpp>

namespace brigadier {

template<typename... Parsers>
    requires(is_parser<Parsers> && ...)
class CommandNode : public ICommandNode {
public:
    CommandNode(
        const std::string_view &name, const std::string_view &description, const std::vector<Argument> &arguments, const std::vector<std::shared_ptr<ICommandNode>> &children,
        const std::vector<std::string> &aliases, const std::function<bool(const TypeHolder)> &permissionPredicate,
        const std::function<void(TypeHolder, typename Parsers::type...)> &callback, const std::function<std::vector<std::string>(TypeHolder)> &suggestionProvider
    ):
        _name(name),
        _description(description),
        _arguments(arguments),
        _children(children),
        _aliases(aliases),
        _permissionPredicate(permissionPredicate),
        _callback(callback),
        _suggestionProvider(suggestionProvider)
    {
    }

public:
    void parse(TypeHolder &source, Reader &reader) const override
    {
        auto start = reader.getCursor();
        try {
            auto entry = reader.readString();
            for (auto &child : _children) {
                if (child->getName() == entry || FIND(child->getAliases(), entry) != child->getAliases().end()) {
                    child->parse(source, reader);
                    return;
                }
            }
            reader.setCursor(start);
            if (_callback == nullptr)
                throw CommandSyntaxException("Invalid command", reader);
            _callback(source, Parsers::parse(reader)...);
        } catch (ReaderException &e) {
            reader.setCursor(start);
            throw e;
        } catch (ParserException &e) {
            reader.setCursor(start);
            throw e;
        }
    }

    const std::vector<std::shared_ptr<ICommandNode>> &getChildren() const override { return _children; }

    std::string_view getName() const override { return _name; }

    std::string_view getUsage() const override { return _description; }

    bool canUse(const TypeHolder &source) override { return _permissionPredicate(source); }

    bool isValidInput(Reader &reader) const override
    {
        auto start = reader.getCursor();
        try {
            auto entry = reader.readString();
            for (auto &child : _children) {
                if (child->getName() == entry)
                    return true;
                if (FIND(child->getAliases(), entry) != child->getAliases().end())
                    return true;
                if (child->isValidInput(reader))
                    return true;
            }
            reader.setCursor(start);
            (Parsers::parse(reader), ...);
            return !reader.canRead() || reader.peek() == ' ';
        } catch (CommandSyntaxException &e) {
            return false;
        }
    }

    std::vector<std::string> listSuggestions(TypeHolder &holder, Reader &reader) const override
    {
        auto name = reader.readString();
        for (auto &child : _children) {
            if (child->getName() == name || FIND(child->getAliases(), name) != child->getAliases().end())
                return child->listSuggestions(holder, reader);
        }
        try {
            reader.setCursor(reader.getCursor() - name.size());
            (Parsers::parse(reader), ...);
            return _suggestionProvider(holder);
        } catch (CommandSyntaxException &e) {
            return {};
        }
    }
    const std::vector<std::string> &getAliases() const override { return _aliases; }

private:
    const std::string_view _name;
    const std::string_view _description;
    const std::vector<Argument> _arguments;
    const std::vector<std::string> _aliases;
    const std::vector<std::shared_ptr<ICommandNode>> _children;
    const std::function<bool(const TypeHolder)> _permissionPredicate;
    const std::function<void(TypeHolder, typename Parsers::type...)> _callback;
    const std::function<std::vector<std::string>(TypeHolder)> _suggestionProvider;
};
} // namespace brigadier
