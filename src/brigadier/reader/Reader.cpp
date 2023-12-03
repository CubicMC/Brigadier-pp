#include <brigadier/exceptions.hpp>
#include <brigadier/reader/Reader.hpp>
#include <regex>
#include <stdexcept>
#include <string>

using namespace brigadier;

namespace brigadier::_util {
static auto INTEGER_REGEX = std::regex("^[-+]?[0-9]+$");
static auto DECIMAL_REGEX = std::regex("^[-+]?[0-9]*\\.?[0-9]+$");
} // namespace brigadier::_util

static std::string makeExpectedValueMessage(Reader *reader, const std::string &expected) { return fmt::format("Expected {} whilst reading {}", expected, reader->getString()); }

static std::string makeInvalidValueMessage(Reader *reader, const std::string &value, const std::string &expected)
{
    return fmt::format("Invalid {} '{}' whilst reading {}", expected, value, reader->getString());
}

bool Reader::readBool()
{
    auto str = this->readUnquotedString();
    if (str.empty())
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "bool"));
    if (str == "false" || str == "0")
        return false;
    else if (str == "true" || str == "1")
        return true;
    throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, str, "bool"));
}

int Reader::readInt()
{
    auto start = getCursor();

    try {
        auto numberRepr = this->readUnquotedString();
        this->skipWhitespace();

        if (!std::regex_match(numberRepr, _util::INTEGER_REGEX))
            throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));

        return std::stoi(numberRepr);
    } catch (const brigadier::CommandSyntaxException &) {
    } catch (const std::out_of_range &) {
    } catch (const std::invalid_argument &) {
    }
    setCursor(start);
    throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
}

long Reader::readLong()
{
    auto start = getCursor();

    try {
        auto numberRepr = this->readUnquotedString();
        this->skipWhitespace();

        if (!std::regex_match(numberRepr, _util::INTEGER_REGEX))
            throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));

        return std::stol(numberRepr);
    } catch (const brigadier::CommandSyntaxException &) {
    } catch (const std::out_of_range &) {
    } catch (const std::invalid_argument &) {
    }
    setCursor(start);
    throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
}

double Reader::readDouble()
{
    auto start = getCursor();

    try {
        auto numberRepr = this->readUnquotedString();
        this->skipWhitespace();

        if (!std::regex_match(numberRepr, _util::DECIMAL_REGEX))
            throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));

        return std::stod(numberRepr);
    } catch (const brigadier::CommandSyntaxException &) {
    } catch (const std::out_of_range &) {
    } catch (const std::invalid_argument &) {
    }
    setCursor(start);
    throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
}

float Reader::readFloat()
{
    auto start = getCursor();

    try {
        auto numberRepr = this->readUnquotedString();
        this->skipWhitespace();

        if (!std::regex_match(numberRepr, _util::DECIMAL_REGEX))
            throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));

        return std::stof(numberRepr);
    } catch (const brigadier::CommandSyntaxException &) {
    } catch (const std::out_of_range &) {
    } catch (const std::invalid_argument &) {
    }
    setCursor(start);
    throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
}

std::string Reader::readString()
{
    if (this->peek() == '"' || this->peek() == '\'')
        return this->readQuotedString();
    return this->readUnquotedString();
}

std::string Reader::readUnquotedString()
{
    auto start = getCursor();

    while (this->canRead() && this->isAllowedInUnquotedString(this->peek()))
        this->skip();
    auto stringRepr = getString().substr(start, getCursor() - start);
    this->skipWhitespace();
    if (stringRepr.empty()) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "string"));
    }
    return stringRepr;
}

std::string Reader::readQuotedString()
{
    if (!this->canRead())
        return "";
    auto next = this->peek();
    if (!this->isQuotedStringStart(next))
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "quote"));
    this->skip();
    return this->readStringUntil(next);
}

std::string Reader::readStringUntil(char terminator)
{
    auto start = this->getCursor();

    while (this->canRead()) {
        auto c = this->peek();
        if (c == '\\') {
            this->skip();
            if (!this->canRead()) {
                setCursor(start);
                throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "escape sequence"));
            }
            this->skip();
        } else if (c == terminator) {
            break;
        }
        this->skip();
    }
    auto end = this->getCursor();
    if (start == end) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "string"));
    }
    if (!this->canRead() || this->peek() != terminator) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, fmt::format("quote '{0}'", terminator)));
    }
    if (this->peek() == terminator)
        this->skip();
    this->skipWhitespace();
    return this->getString().substr(start, end - start);
}
