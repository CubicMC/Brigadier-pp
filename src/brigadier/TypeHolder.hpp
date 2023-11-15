#pragma once

#include <memory>
#include <type_traits>
#include <typeinfo>

class TypeHolder {
public:
    TypeHolder():
        _value(nullptr),
        _type(typeid(void))
    {
    }

    template<typename T>
    TypeHolder(T *value):
        _value(value),
        _type(typeid(T))
    {
    }

    template<typename T>
        requires(!std::is_same_v<T, TypeHolder>)
    TypeHolder(T &value):
        _value(&value),
        _type(typeid(T))
    {
    }

    TypeHolder(const TypeHolder &) = default;
    TypeHolder(TypeHolder &&) = default;

    template<typename T>
    T *getAs()
    {
        if (typeid(T) != _type) {
            return nullptr;
        }
        return static_cast<T *>(_value);
    }

    template<typename T>
    const T *getAs() const
    {
        if (typeid(T) != _type) {
            return nullptr;
        }
        return static_cast<const T *>(_value);
    }

    template<typename T>
    constexpr bool is() const
    {
        return typeid(T) == _type;
    }

    void *get() { return _value; }
    const void *get() const { return _value; }

private:
    void *_value;
    const std::type_info &_type;
};
