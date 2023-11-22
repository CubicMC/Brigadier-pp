#pragma once

#include <memory>
#include <type_traits>
#include <typeinfo>

namespace brigadier {
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

    template<typename T>
    TypeHolder(T *value):
        _value(value),
        _type(typeid(T))
    {
    }

    template<typename T>
    TypeHolder(T &value):
        _value(&value),
        _type(typeid(T))
    {
    }

    TypeHolder(TypeHolder &value):
        _value(value._value),
        _type(value._type)
    {
    }

    TypeHolder(TypeHolder *value):
        _value(value->_value),
        _type(value->_type)
    {
    }

    TypeHolder(std::nullptr_t):
        _value(nullptr),
        _type(typeid(void))
    {
    }

    TypeHolder(const TypeHolder &) = default;
    TypeHolder(TypeHolder &&) = default;

    /**
     * @brief Get the stored object as T
     *
     * T must be the same type as the stored object, not a parent class or a child class
     * otherwise, nullptr will be returned
     *
     * @tparam T
     * @return T*
     */
    template<typename T>
    T *getAs()
    {
        if (typeid(T) != _type) {
            return nullptr;
        }
        return static_cast<T *>(_value);
    }

    /**
     * @brief Get the stored object as T
     *
     * T must be the same type as the stored object, not a parent class or a child class
     * otherwise, nullptr will be returned
     *
     * @tparam T
     * @return const T*
     */
    template<typename T>
    const T *getAs() const
    {
        if (typeid(T) != _type) {
            return nullptr;
        }
        return static_cast<const T *>(_value);
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

private:
    void *_value;
    const std::type_info &_type;
};
} // namespace brigadier
