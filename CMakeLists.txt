cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Brigadier VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

if(GTEST) # if the flag GTEST is true
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        rapidcheck
        GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
        GIT_TAG a5724ea5b0b00147109b0605c377f1e54c353ba2 # The latest rapidcheck commit at the time of writing.
    )
    option(RC_ENABLE_GTEST "Enable Google Test integration for RapidCheck library" ON)

    FetchContent_MakeAvailable(
        rapidcheck
        googletest
    )

    set(ENABLE_TESTING ON CACHE BOOL "" FORCE)
    enable_testing()
    include(GoogleTest)
endif()

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)
FetchContent_MakeAvailable(fmt)

add_library(Brigadier)
add_library(Brigadier::Brigadier ALIAS Brigadier)

target_link_libraries(Brigadier PUBLIC fmt::fmt)

target_include_directories(Brigadier PUBLIC src)

add_subdirectory(src)
add_subdirectory(tests)
