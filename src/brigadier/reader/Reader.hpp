#pragma once

#include <string>

namespace brigadier {
class Reader {
public:
    Reader() = default;
    virtual ~Reader() = default;

    virtual std::string getString() const = 0;
    virtual size_t getRemainingLength() const = 0;
    virtual size_t getTotalLength() const = 0;
    virtual size_t getCursor() const = 0;
    virtual std::string getRead() const = 0;
    virtual std::string getRemaining() const = 0;

    virtual bool canRead(size_t length) const = 0;
    virtual bool canRead() const = 0;
    virtual char peek() const = 0;
    virtual char peek(size_t offset) const = 0;

    virtual void skip() = 0;

    virtual bool isAllowedNumberPart(char c) const { return c >= '0' && c <= '9' || c == '.' || c == '-'; }

    virtual bool isQuotedStringStart(char c) const { return c == '"' || c == '\''; }

    virtual bool isAllowedInUnquotedString(char c) const
    {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '-' || c == '.' || c == '+';
    }

    virtual bool isSpace(char c) const { return std::isspace(c); }

    virtual void skipWhitespace()
    {
        while (this->canRead() && this->isSpace(this->peek()))
            this->skip();
    }

    virtual bool readBool();
    virtual int readInt();
    virtual long readLong();
    virtual double readDouble();
    virtual float readFloat();
    virtual std::string readString();
    virtual std::string readUnquotedString();
    virtual std::string readQuotedString();
    virtual std::string readStringUntil(char terminator);
};

} // namespace brigadier
