add_executable(
    list10
    src/list10.cpp
)
target_link_libraries(
    list10
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list10)