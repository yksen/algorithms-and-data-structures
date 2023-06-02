add_executable(
    list6
    src/list6.cpp
)
target_link_libraries(
    list6
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list6)