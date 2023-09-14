#pragma once

#include <brigadier/CommandNode.hpp>
#include <brigadier/argument.hpp>
#include <brigadier/reader/Reader.hpp>
#include <type_traits>
#include <utility>
#include <vector>

namespace brigadier {

/**
 * @brief a class that holds a list of arguments
 *
 * @note beaware that the argument will be constructed on the heap
 */
class ArgumentsHolder {
public:
    ArgumentsHolder() = default;
    virtual ~ArgumentsHolder() = default;

    /**
     * @brief construct a new Argument object and add it to the list
     *
     * @tparam T
     * @tparam Args
     * @param args
     * @return ArgumentsHolder&
     */
    template<typename T, typename... Args>
    requires std::is_base_of_v<Argument, T>
    ArgumentsHolder &addArgument(Args &&... args)
    {
        this->_arguments.emplace_back(new T(std::forward<Args>(args)...));
        return *this;
    }

    /**
     * @brief add an already constructed argument to the list
     *
     * @param argument
     * @return ArgumentsHolder&
     */
    ArgumentsHolder &addArgument(Argument *argument)
    {
        this->_arguments.emplace_back(argument);
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * @note this function will reserve the space for the arguments @see addArguments
     *
     * @param arguments
     * @return ArgumentsHolder&
     */
    ArgumentsHolder &addArgumentsR(const std::vector<Argument *> &arguments)
    {
        this->_arguments.reserve(this->_arguments.size() + arguments.size());
        return this->addArguments(arguments);
    }

    /**
     * @brief add a list of arguments to the list
     *
     * @note this function will not reserve the space for the arguments @see addArgumentsR
     *
     * @param arguments
     * @return ArgumentsHolder&
     */
    ArgumentsHolder &addArguments(const std::vector<Argument *> &arguments)
    {
        this->_arguments.insert(this->_arguments.end(), arguments.begin(), arguments.end());
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * @note this function will reserve the space for the arguments @see addArguments
     *
     * @param arguments
     * @return ArgumentsHolder&
     */
    ArgumentsHolder &addArgumentsR(const ArgumentsHolder &arguments)
    {
        this->_arguments.reserve(this->_arguments.size() + arguments._arguments.size());
        return this->addArguments(arguments);
    }

    /**
     * @brief add a list of arguments to the list
     *
     * @note this function will not reserve the space for the arguments @see addArgumentsR
     *
     * @param arguments
     * @return ArgumentsHolder&
     */
    ArgumentsHolder &addArguments(const ArgumentsHolder &arguments)
    {
        this->_arguments.insert(this->_arguments.end(), arguments._arguments.begin(), arguments._arguments.end());
        return *this;
    }

protected:
    std::vector<Argument *> _arguments;
};


class Command : public CommandNode {
public:
    Command(const std::string &name):
        CommandNode(name)
    {
    }
    virtual ~Command() = default;

    /**
     * @brief construct a new Argument object and add it to the list
     *
     * the argument will be constructed with the given parameters
     * keep in mind that if you want to add multiple options for the same command
     * this method will always add to the last one
     *
     * beaware that the argument will be constructed on the heap
     *
     * @tparam T
     * @tparam Args
     * @param args
     * @return requires&
     */
    template<typename T, typename... Args>
    requires std::is_base_of_v<Argument, T>
    Command &addArgument(Args &&... args)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArgument<T>(std::forward<Args>(args)...);
        return *this;
    }

    /**
     * @brief add an already constructed argument to the list
     *
     * the argument will be added to the last one
     *
     * @param argument
     * @return Command&
     */
    Command &addArgument(Argument *argument)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArgument(argument);
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * the arguments will be added to the last one
     *
     * @note this function will reserve the space for the arguments @see addArguments
     *
     * @param arguments
     * @return Command&
     */
    Command &addArgumentsR(const std::vector<Argument *> &arguments)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArgumentsR(arguments);
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * the arguments will be added to the last one
     *
     * @note this function will not reserve the space for the arguments @see addArgumentsR
     *
     * @param arguments
     * @return Command&
     */
    Command &addArguments(const std::vector<Argument *> &arguments)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArguments(arguments);
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * the arguments will be added to the last one
     *
     * @note this function will reserve the space for the arguments @see addArguments
     *
     * @param arguments
     * @return Command&
     */
    Command &addArgumentsR(const ArgumentsHolder &arguments)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArgumentsR(arguments);
        return *this;
    }

    /**
     * @brief add a list of arguments to the list
     *
     * the arguments will be added to the last one
     *
     * @note this function will not reserve the space for the arguments @see addArgumentsR
     *
     * @param arguments
     * @return Command&
     */
    Command &addArguments(const ArgumentsHolder &arguments)
    {
        this->_arguments[this->_currentArgumentOptionIndex].addArguments(arguments);
        return *this;
    }

    /**
     * @brief add a new option to the command
     *
     * a command can have multiple options
     * each option can have multiple arguments
     *
     * exemple:
     * command <arg1> <arg2>
     * command <arg1> <arg2> <arg3>
     *
     * where each argument can be of different type
     *
     * @param name
     * @return Command&
     */

    /**
     * @brief
     *
     * @param reader
     */
    virtual void execute(Reader &reader) = 0;

private:
    size_t _currentArgumentOptionIndex = 0;
    std::vector<ArgumentsHolder> _arguments;
};

} // namespace brigadier
