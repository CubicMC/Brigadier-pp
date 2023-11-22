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

namespace brigadier {

/**
 * @brief Construct a new Command Node object
 *
 * This is the base class for all command nodes. It is used to parse the command and execute the callback.
 *
 * @warning Prefer using the `brigadier::CommandNodeBuilder` class to create command nodes.
 *
 * @tparam Parsers A list of parsers that will be used to parse the arguments
 */
template<typename... Parsers>
    requires(is_parser<Parsers> && ...)
class CommandNode : public ICommandNode {
public:
    /**
     * @brief Construct a new Command Node object
     *
     * @private
     *
     * @param name
     * @param description
     * @param arguments
     * @param children
     * @param aliases
     * @param permissionPredicate
     * @param callback
     * @param suggestionProvider
     */
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
    /**
     * @brief Parse and execute the command
     *
     * @throw CommandSyntaxException If the command is invalid
     * @throw ParserException If a parser fails
     *
     * @param source The source of the command
     * @param reader The reader to parse the command from
     */
    void parse(TypeHolder &source, Reader &reader) const override
    {
        auto start = reader.getCursor();
        try {
            auto entry = reader.readString();
            for (auto &child : _children) {
                if (child->getName() == entry || std::find(child->getAliases().begin(), child->getAliases().end(), entry) != child->getAliases().end()) {
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

    /**
     * @brief Get the Children object
     *
     * @return const std::vector<std::shared_ptr<ICommandNode>>&
     */
    const std::vector<std::shared_ptr<ICommandNode>> &getChildren() const override { return _children; }

    /**
     * @brief Get the Name object
     *
     * @return std::string_view
     */
    std::string_view getName() const override { return _name; }

    /**
     * @brief Get the Usage object
     *
     * @return std::string_view
     */
    std::string_view getUsage() const override { return _description; }

    /**
     * @brief Execute the command predicate to check if source can use this command
     *
     * @param source
     * @return bool
     */
    bool canUse(const TypeHolder &source) override { return _permissionPredicate(source); }

    /**
     * @brief Check if the input is valid
     *
     * @param reader
     * @return bool
     */
    bool isValidInput(Reader &reader) const override
    {
        auto start = reader.getCursor();
        try {
            auto entry = reader.readString();
            for (auto &child : _children) {
                if (child->getName() == entry)
                    return true;
                if (std::find(child->getAliases().begin(), child->getAliases().end(), entry) != child->getAliases().end())
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

    /**
     * @brief List all the suggestions for the input
     *
     * @param holder
     * @param reader
     * @return std::vector<std::string>
     */
    std::vector<std::string> listSuggestions(TypeHolder &holder, Reader &reader) const override
    {
        auto name = reader.readString();
        for (auto &child : _children) {
            if (child->getName() == name || std::find(child->getAliases().begin(), child->getAliases().end(), name) != child->getAliases().end())
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

    /**
     * @brief Get the Aliases object
     *
     * @return const std::vector<std::string>&
     */
    const std::vector<std::string> &getAliases() const override { return _aliases; }

private:
    /**
     * @brief Construct a new Command Node object
     *
     * @private
     */
    CommandNode() = delete;

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
