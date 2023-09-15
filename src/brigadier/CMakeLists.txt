target_sources(Brigadier
    PRIVATE
        # CommandNode.cpp
    PUBLIC
        CommandNode.hpp
        Argument.hpp
        Command.hpp
        Dispatcher.hpp
        exceptions.hpp
)

add_subdirectory(reader)
add_subdirectory(parser)
