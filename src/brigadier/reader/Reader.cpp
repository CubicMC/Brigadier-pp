#include <brigadier/exceptions.hpp>
#include <brigadier/reader/Reader.hpp>
#include <stdexcept>
#include <string>

using namespace brigadier;

static std::string makeExpectedValueMessage(Reader *reader, const std::string &expected) { return fmt::format("Expected {} whilst reading {}", expected, reader->getString()); }

static std::string makeInvalidValueMessage(Reader *reader, const std::string &value, const std::string &expected)
{
    return fmt::format("Invalid {} '{}' whilst reading {}", expected, value, reader->getString());
}

bool Reader::readBool() { return this->readString() == "true"; }

int Reader::readInt()
{
    auto start = getCursor();

    while (this->canRead() && this->isAllowedNumberPart(this->peek()))
        this->skip();
    if (this->canRead() && !this->isAllowedNumberPart(this->peek()) && !this->isSpace(this->peek())) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
    }
    auto numberRepr = getString().substr(start, getCursor() - start);
    if (numberRepr.empty()) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "int"));
    }
    this->skipWhitespace();
    try {
        return std::stoi(numberRepr);
    } catch (const std::out_of_range &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));
    } catch (const std::invalid_argument &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "int"));
    }
}

long Reader::readLong()
{
    auto start = getCursor();

    while (this->canRead() && this->isAllowedNumberPart(this->peek()))
        this->skip();
    if (this->canRead() && !this->isAllowedNumberPart(this->peek()) && !this->isSpace(this->peek())) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "long"));
    }
    auto numberRepr = getString().substr(start, getCursor() - start);
    if (numberRepr.empty()) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "long"));
    }
    this->skipWhitespace();
    try {
        return std::stol(numberRepr);
    } catch (const std::out_of_range &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "long"));
    } catch (const std::invalid_argument &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "long"));
    }
}

double Reader::readDouble()
{
    auto start = getCursor();

    while (this->canRead() && this->isAllowedNumberPart(this->peek()))
        this->skip();
    if (this->canRead() && !this->isAllowedNumberPart(this->peek()) && !this->isSpace(this->peek())) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "double"));
    }
    auto numberRepr = getString().substr(start, getCursor() - start);
    if (numberRepr.empty()) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "double"));
    }
    this->skipWhitespace();
    try {
        return std::stod(numberRepr);
    } catch (const std::out_of_range &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "double"));
    } catch (const std::invalid_argument &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "double"));
    }
}

float Reader::readFloat()
{
    auto start = getCursor();

    while (this->canRead() && this->isAllowedNumberPart(this->peek()))
        this->skip();
    if (this->canRead() && !this->isAllowedNumberPart(this->peek()) && !this->isSpace(this->peek())) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "float"));
    }
    auto numberRepr = getString().substr(start, getCursor() - start);
    if (numberRepr.empty()) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeExpectedValueMessage(this, "float"));
    }
    this->skipWhitespace();
    try {
        return std::stof(numberRepr);
    } catch (const std::out_of_range &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "float"));
    } catch (const std::invalid_argument &) {
        setCursor(start);
        throw brigadier::CommandSyntaxException(makeInvalidValueMessage(this, numberRepr, "float"));
    }
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
            this->skip();
            break;
        }
        this->skip();
    }
    auto end = this->getCursor();
    this->skipWhitespace();
    return this->getString().substr(start, end - start);
}
