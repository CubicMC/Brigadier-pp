#pragma once

#include "brigadier/Argument.hpp"
#include "brigadier/Parser.hpp"
#include <brigadier/CommandNode.hpp>

/**
 * @brief A builder class to create command nodes
 *
 * This class is used to create command nodes. It is used to create a command node with a name and a description, and then add arguments to it.
 *
 * Here are some examples of how to use this class:
 *
 * @code
 * Registry registry;
 *
 * registry.add(CommandNodeBuilder("test", "A test command")
 *    .expectArg<NumberParser<int>>("int", "An integer argument")
 *    .withPermission([](const TypeHolder &source) { return true; })
 *    .execute([](const TypeHolder &source, int i) { std::cout << "You entered " << i << std::endl; })
 * );
 * @endcode
 *
 * @code
 * Registry registry;
 *
 * registry.add(CommandNodeBuilder("test", "A test command")
 *    .expectArg<NumberParser<int>>("int", "An integer argument")
 *    .withPermission([](const TypeHolder &source) { return true; })
 *    .execute([](const TypeHolder &source, int i) { std::cout << "You entered " << i << std::endl; })
 *    .add(CommandNodeBuilder("subcommand", "A subcommand")
 *      .expectArg<NumberParser<int>>("int", "An integer argument")
 *      .execute([](const TypeHolder &source, int i) { std::cout << "[subcommand] You entered " << i << std::endl; })
 *    )
 * );
 * @endcode
 *
 * @tparam _Parsers A list of parsers that will be used to parse the arguments
 */
namespace brigadier {
template<typename... _Parsers>
    requires(is_parser<_Parsers> && ...)
class CommandNodeBuilder {
    template<typename... T>
        requires(is_parser<T> && ...)
    friend class CommandNodeBuilder;

public:
    /**
     * @brief Construct a new Command Node Builder object
     *
     * @warning Do not pass parsers to this constructor, use expectArg instead
     *
     * @param name
     * @param description
     */
    CommandNodeBuilder(const std::string_view &name, const std::string &description = ""):
        _name(name),
        _description(description),
        _children(),
        _aliases(),
        _permissionPredicate(),
        _callback(),
        _suggestionProvider()
    {
        static_assert(sizeof...(_Parsers) == 0, "Don't provide parsers to the CommandNodeBuilder, use expectArg");
    }

    CommandNodeBuilder(const CommandNodeBuilder &) = delete;
    CommandNodeBuilder<_Parsers...> &operator=(const CommandNodeBuilder &) = delete;

    /**
     * @brief Add an argument to the command node
     *
     * @tparam T The parser to use to parse the argument
     * @param name
     * @param description
     * @param required
     * @return brigadier::CommandNodeBuilder<_Parsers..., T>
     */
    template<typename T>
        requires is_parser<T>
    CommandNodeBuilder<_Parsers..., T> expectArg(const std::string &name, const std::string &description = "", bool required = false)
    {
        return expectArg<T>(Argument {name, description, required});
    }

    /**
     * @see CommandNodeBuilder::expectArg
     */
    template<typename T>
        requires is_parser<T>
    CommandNodeBuilder<_Parsers..., T> expectArg(const Argument &argument)
    {
        // static_assert(_callback == nullptr, "Cannot move a builder with a callback, use expectArg before execute");
        return CommandNodeBuilder<_Parsers..., T>(*this, argument);
    }

    /**
     * @brief Set the callback to execute when the command is parsed
     *
     * @param callback
     * @return CommandNodeBuilder&
     */
    CommandNodeBuilder &execute(std::function<void(TypeHolder, typename _Parsers::type...)> callback)
    {
        _callback = std::move(callback);
        return *this;
    }

    /**
     * @brief Add an alias to the command node
     *
     * @param alias
     * @return CommandNodeBuilder&
     */
    CommandNodeBuilder &alias(const std::string &alias)
    {
        _aliases.emplace_back(alias);
        return *this;
    }

    /**
     * @brief Set the permission predicate
     *
     * @param permissionPredicate
     * @return CommandNodeBuilder&
     */
    CommandNodeBuilder &withPermission(std::function<bool(const TypeHolder)> permissionPredicate)
    {
        _permissionPredicate = std::move(permissionPredicate);
        return *this;
    }

    /**
     * @brief Add a child to the command node
     *
     * @param child
     * @return CommandNodeBuilder&
     */
    CommandNodeBuilder &add(std::shared_ptr<ICommandNode> child)
    {
        _children.emplace_back(std::move(child));
        return *this;
    }

    /**
     * @brief Set the suggestion provider
     *
     * @param suggestionProvider
     * @return CommandNodeBuilder&
     */
    CommandNodeBuilder &suggestionBuilder(std::function<std::vector<std::string>(TypeHolder)> suggestionProvider)
    {
        _suggestionProvider = std::move(suggestionProvider);
        return *this;
    }

    /**
     * @brief Build the command node
     *
     * @return std::shared_ptr<ICommandNode>
     */
    std::shared_ptr<ICommandNode> build() const
    {
        return std::make_shared<CommandNode<_Parsers...>>(_name, _description, _arguments, _children, _aliases, _permissionPredicate, _callback, _suggestionProvider);
    }

    /**
     * @brief Build the command node
     *
     * @return std::shared_ptr<ICommandNode>
     */
    operator std::shared_ptr<ICommandNode>() const { return build(); }

private:
    /**
     * @brief Construct a new Command Node Builder object to add an argument
     *
     * @private
     *
     * @tparam Args
     * @param builder
     * @param argument
     */
    template<typename... Args>
    CommandNodeBuilder(const CommandNodeBuilder<Args...> &builder, const Argument &argument):
        _name(builder._name),
        _description(builder._description),
        _arguments(std::move(builder._arguments)),
        _children(std::move(builder._children)),
        _aliases(std::move(builder._aliases)),
        _permissionPredicate(std::move(builder._permissionPredicate)),
        _callback(),
        _suggestionProvider(std::move(builder._suggestionProvider))
    {
        this->_arguments.emplace_back(argument);
    }

private:
    const std::string_view _name;
    const std::string_view _description;
    std::vector<Argument> _arguments;
    std::vector<std::shared_ptr<ICommandNode>> _children;
    std::vector<std::string> _aliases;
    std::function<bool(const TypeHolder)> _permissionPredicate;
    std::function<void(TypeHolder, typename _Parsers::type...)> _callback;
    std::function<std::vector<std::string>(TypeHolder)> _suggestionProvider;
};
} // namespace brigadier
