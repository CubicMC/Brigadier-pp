#include "brigadier/exceptions.hpp"
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
        if (node->getName() == cmd || std::find(node->getAliases().begin(), node->getAliases().end(), cmd) != node->getAliases().end()) {
            node->parse(source, reader);
            return;
        }
    }
    throw CommandSyntaxException("Unknown command", reader);
}

bool brigadier::Registry::isValidInput(const std::string &input) const
{
    StringReader reader(input);
    return isValidInput(reader);
}

bool brigadier::Registry::isValidInput(Reader &input) const
{
    auto start = input.getCursor();
    try {
        for (auto &node : _nodes) {
            auto entry = input.readString();
            if (node->getName() == entry || std::find(node->getAliases().begin(), node->getAliases().end(), entry) != node->getAliases().end()) {
                auto result = node->isValidInput(input);
                input.setCursor(start);
                return result;
            }
        }
    } catch (const CommandSyntaxException &e) {
    } catch (const ParserException &e) {
    } catch (const ReaderException &e) {
    }
    input.setCursor(start);
    return false;
}

std::vector<std::string> brigadier::Registry::listSuggestions(TypeHolder &holder, Reader &reader) const
{
    std::string name = reader.readString();
    for (auto &node : _nodes) {
        if (node->getName() == name || std::find(node->getAliases().begin(), node->getAliases().end(), name) != node->getAliases().end())
            return node->listSuggestions(holder, reader);
    }
    return {};
}
