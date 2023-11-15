#pragma once

#include "brigadier/Argument.hpp"
#include "brigadier/Parser.hpp"
#include <brigadier/CommandNode.hpp>

namespace brigadier {
template<typename... _Parsers>
    requires(is_parser<_Parsers> && ...)
class CommandNodeBuilder {
    template<typename... T>
        requires(is_parser<T> && ...)
    friend class CommandNodeBuilder;

public:
    CommandNodeBuilder(const std::string_view &name, const std::string &description = ""):
        _name(name),
        _description(description),
        _children(),
        _aliases(),
        _permissionPredicate(),
        _callback(),
        _suggestionProvider()
    {
    }

    CommandNodeBuilder(const CommandNodeBuilder &) = delete;
    CommandNodeBuilder<_Parsers...> &operator=(const CommandNodeBuilder &) = delete;

    template<typename T>
        requires is_parser<T>
    CommandNodeBuilder<_Parsers..., T> expectArg(const std::string &name, const std::string &description = "", bool required = false)
    {
        return expectArg<T>(Argument {name, description, required});
    }

    template<typename T>
        requires is_parser<T>
    CommandNodeBuilder<_Parsers..., T> expectArg(const Argument &argument)
    {
        // static_assert(_callback == nullptr, "Cannot move a builder with a callback, use expectArg before execute");
        return CommandNodeBuilder<_Parsers..., T>(*this, argument);
    }

    CommandNodeBuilder &execute(std::function<void(TypeHolder, typename _Parsers::type...)> callback)
    {
        _callback = std::move(callback);
        return *this;
    }

    CommandNodeBuilder &alias(const std::string &alias)
    {
        _aliases.emplace_back(alias);
        return *this;
    }

    CommandNodeBuilder &withPermission(std::function<bool(const TypeHolder)> permissionPredicate)
    {
        _permissionPredicate = std::move(permissionPredicate);
        return *this;
    }

    CommandNodeBuilder &add(std::shared_ptr<ICommandNode> child)
    {
        _children.emplace_back(std::move(child));
        return *this;
    }

    CommandNodeBuilder &suggestionBuilder(std::function<std::vector<std::string>(TypeHolder)> suggestionProvider)
    {
        _suggestionProvider = std::move(suggestionProvider);
        return *this;
    }

    std::shared_ptr<ICommandNode> build() const
    {
        return std::make_shared<CommandNode<_Parsers...>>(_name, _description, _arguments, _children, _aliases, _permissionPredicate, _callback, _suggestionProvider);
    }

    operator std::shared_ptr<ICommandNode>() const { return build(); }

private:
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
