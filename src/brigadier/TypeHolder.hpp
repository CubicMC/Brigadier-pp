#pragma once

#include <brigadier/exceptions.hpp>
#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>

namespace brigadier {
class TypeHolder;
namespace _util {
template<typename T>
concept _isnt_th = !std::is_same_v<TypeHolder, T>;

#if defined(__clang__) || defined(__GNUC__)
inline std::string _demangle(const std::type_info &type_info)
{
    int status = -1;
    char *rName = abi::__cxa_demangle(type_info.name(), nullptr, nullptr, &status);
    std::string name = rName;
    std::free(rName);
    return status == 0 ? name : type_info.name();
}
#define GET_NAME(type_info) brigadier::_util::_demangle(type_info)
#else
#define GET_NAME(type_info) type_info.name();
#endif
} // namespace _util

/**
 * @brief A type holder, used to store a pointer to an object and its type
 */
class TypeHolder {
public:
    TypeHolder():
        _value(nullptr),
        _type(typeid(void))
    {
    }

    TypeHolder(std::nullptr_t):
        _value(nullptr),
        _type(typeid(void))
    {
    }

    TypeHolder(const TypeHolder &other):
        _value(other._value),
        _type(other._type)
    {
    }

    template<_util::_isnt_th T>
    TypeHolder(T &value):
        _value(&value),
        _type(typeid(T))
    {
    }

    TypeHolder(TypeHolder &&) = default;

    TypeHolder &operator=(TypeHolder &) = delete;
    TypeHolder &operator=(const TypeHolder &) = delete;
    TypeHolder &operator=(TypeHolder &&) = delete;

    /**
     * @brief Get the stored object as T
     *
     * T must be the same type as the stored object, not a parent class or a child class
     * otherwise, nullptr will be returned
     *
     * @throw InvalidTypeException The stored object is not of type T
     *
     * @tparam T
     * @return T&
     */
    template<typename T>
    T &getAs()
    {
        static_assert(!std::is_same_v<TypeHolder, T>, "T must not be TypeHolder");
        if (typeid(T) != _type) {
            throw InvalidTypeException(fmt::format("Invalid type: expected {}, got {}", GET_NAME(_type), GET_NAME(typeid(T))));
        }
        return *static_cast<T *>(_value);
    }

    /**
     * @brief Get the stored object as T
     *
     * T must be the same type as the stored object, not a parent class or a child class
     * otherwise, nullptr will be returned
     *
     * @throw InvalidTypeException The stored object is not of type T
     *
     * @tparam T
     * @return const T&
     */
    template<typename T>
    const T &getAs() const
    {
        if (typeid(T) != _type) {
            throw InvalidTypeException(fmt::format("Invalid type: expected {}, got {}", GET_NAME(_type), GET_NAME(typeid(T))));
        }
        return *static_cast<const T *>(_value);
    }

    /**
     * @brief Check if the stored object is of type T
     *
     * @tparam T
     * @return bool
     */
    template<typename T>
    constexpr bool is() const
    {
        return typeid(T) == _type;
    }

    /**
     * @brief Get the stored object
     *
     * @return void*
     */
    void *get() { return _value; }

    /**
     * @brief Get the stored object
     *
     * @return const void*
     */
    const void *get() const { return _value; }

    /**
     * @brief The object is valid if it is not nullptr
     *
     * @return bool
     */
    operator bool() const { return _value != nullptr; }

    /**
     * @brief Compare two TypeHolders
     *
     * @return bool
     */
    bool operator==(const TypeHolder &other) const { return _value == other._value && _type == other._type; }

private:
    void *_value;
    const std::type_info &_type;
};
} // namespace brigadier
