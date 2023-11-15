#include <brigadier/Registry.hpp>

brigadier::Registry &brigadier::Registry::add(const std::shared_ptr<brigadier::ICommandNode> &node)
{
    _nodes.emplace_back(node);
    return *this;
}

void brigadier::Registry::parse(const std::string &command) const
{
    StringReader reader(command);
    parse(reader);
}

void brigadier::Registry::parse(Reader &reader) const
{
    TypeHolder holder;
    parse(holder, reader);
}

void brigadier::Registry::parse(TypeHolder holder, const std::string &command)
{
    StringReader reader(command);
    parse(holder, reader);
}

void brigadier::Registry::parse(TypeHolder &source, Reader &reader) const
{
    reader.skipWhitespace();
    auto cmd = reader.readString();
    for (auto &node : _nodes) {
        if (node->getName() == cmd || FIND(node->getAliases(), cmd) != node->getAliases().end()) {
            node->parse(source, reader);
            return;
        }
    }
    throw CommandSyntaxException("Unknown command", reader);
}

bool brigadier::Registry::isValidInput(Reader &input) const
{
    for (auto &node : _nodes) {
        if (node->isValidInput(input))
            return true;
    }
    return false;
}

std::vector<std::string> brigadier::Registry::listSuggestions(TypeHolder &holder, Reader &reader) const
{
    std::string name = reader.readString();
    for (auto &node : _nodes) {
        if (node->getName() == name || FIND(node->getAliases(), name) != node->getAliases().end())
            return node->listSuggestions(holder, reader);
    }
    return {};
}
