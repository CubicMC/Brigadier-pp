add_executable(tests
    stringReader.cpp
    # Dispatcher.cpp
    test.cpp
)

# target_compile_definitions(parser_test PRIVATE
#     GTEST_REMOVE_LEGACY_TEST_CASEAPI_=1
# )

target_link_libraries(tests PRIVATE
    ${PROJECT_NAME}::${PROJECT_NAME}
    GTest::gtest_main
    GTest::gmock
    rapidcheck
    rapidcheck_gtest
)

target_link_directories(tests PRIVATE ${CMAKE_BINARY_DIR}/lib)

gtest_discover_tests(tests)
