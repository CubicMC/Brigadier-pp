#pragma once

#include "brigadier/CommandNode.hpp"
#include <vector>

namespace brigadier {

class SubCommand : public CommandNode {
public:
    SubCommand(const std::string &name):
        CommandNode(name)
    {
    }
    virtual ~SubCommand() = default;

    template<typename T, typename... Args>
    requires std::is_base_of_v<CommandNode, T>
    CommandNode& addSubcommand(Args&&... args)
    {
        this->_subcommands.emplace_back(T(std::forward<Args>(args)...));
    }

private:
    std::vector<CommandNode> _subcommands;
};

} // namespace brigadier
