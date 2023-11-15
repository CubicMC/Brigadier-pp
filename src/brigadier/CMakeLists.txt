target_sources(${PROJECT_NAME}
    PRIVATE
        Registry.cpp
    PUBLIC
        Argument.hpp
        CommandNode.hpp
        CommandNodeBuilder.hpp
        exceptions.hpp
        options.hpp
        Parser.hpp
        Registry.hpp
        TypeHolder.hpp
        parser.hpp
        reader.hpp
)

add_subdirectory(reader)
add_subdirectory(parser)
