#pragma once

#include <algorithm>
#include <bits/ranges_algobase.h>
#include <brigadier/CommandNode.hpp>
#include <brigadier/options.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace brigadier {

class Dispatcher {
public:
    template<typename T>
    class ParseResults {
        friend class Dispatcher;

    public:
        ~ParseResults() = default;

    private:
        ParseResults(const CommandNode &command, const std::vector<std::string> &arguments, T &source):
            _command(command),
            _arguments(arguments),
            _source(source)
        {
        }

        size_t size() const { return _arguments.size(); }
        const std::string &operator[](size_t index) const { return _arguments[index]; }

    private:
        const CommandNode &_command;
        const std::vector<std::string> _arguments;
        T &_source;
    };

public:
    Dispatcher() = default;
    ~Dispatcher()
    {
        for (auto &[_, command] : commands) {
            delete command;
        }
    }
    Dispatcher(const Dispatcher &) = delete;
    Dispatcher &operator=(const Dispatcher &) = delete;
    Dispatcher(Dispatcher &&) = default;
    Dispatcher &operator=(Dispatcher &&) = default;

    template<typename T, typename... Args>
        requires(std::is_base_of<CommandNode, T>::value)
    void registerCommand(const std::string &name, Args &&...args)
    {
        commands.emplace(std::make_pair(name, new T(name, args...)));
    }

    template<typename T>
    void execute(const std::string &input, T &source);

    template<typename T>
    void execute(const ParseResults<T> &results);

    template<typename T>
    ParseResults<T> parse(const std::string &input, T &source)
    {
        std::vector<std::string> arguments;
        auto begin = input.begin();

        for (auto it = std::find_first_of(input.begin(), input.end(), COMMAND_SEPARATOR.begin(), COMMAND_SEPARATOR.end()); it != input.end();
            begin = it, it = std::find_first_of(input.begin(), input.end(), COMMAND_SEPARATOR.begin(), COMMAND_SEPARATOR.end())) {
            arguments.emplace_back(std::string(&*begin, std::distance(begin, it)));
        }

        auto cmd = std::find_if(commands.begin(), commands.end(), [arguments](const auto &pair) {
            return arguments[0] == pair.first;
        });

        if (cmd == commands.end())
            throw std::runtime_error("Command not found"); // TODO(Miki): custom error

        return ParseResults<T>(*cmd->second, arguments, source);
    }

private:
    std::unordered_map<std::string, CommandNode *> commands;
};
} // namespace brigadier
