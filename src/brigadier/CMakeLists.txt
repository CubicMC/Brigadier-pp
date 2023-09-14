target_sources(Brigadier
    PRIVATE
        CommandNode.cpp
    PUBLIC
        CommandNode.hpp
        Dispatcher.hpp
        exceptions.hpp
)

add_subdirectory(parser)
add_subdirectory(reader)
