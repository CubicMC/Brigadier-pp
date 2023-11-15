#pragma once

#include <brigadier/reader/Reader.hpp>
#include <stdexcept>

namespace brigadier {
class StringReader final : public Reader {
public:
    StringReader(const std::string &str, size_t cursor = 0):
        _string(str),
        _cursor(cursor)
    {
    }
    ~StringReader() = default;

    std::string getString() const override { return _string; }
    size_t getRemainingLength() const override { return _string.size() - _cursor; }
    size_t getTotalLength() const override { return _string.size(); }
    size_t getCursor() const override { return _cursor; }
    std::string getRead() const override { return _string.substr(0, _cursor); }
    std::string getRemaining() const override { return _string.substr(_cursor); }
    void setCursor(size_t cursor) override { _cursor = cursor; }

    bool canRead(size_t length) const override { return _cursor + length < _string.size(); }
    bool canRead() const override { return _cursor < _string.size(); }
    char peek() const override { return _string[_cursor]; }
    char peek(size_t offset) const override
    {
        if (_cursor + offset >= _string.size())
            throw std::out_of_range("Cannot peek past end of string");
        return _string[_cursor + offset];
    }
    void skip() override { _cursor++; }

private:
    const std::string _string;
    size_t _cursor;
};

} // namespace brigadier
