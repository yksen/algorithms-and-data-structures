add_executable(
    list5
    src/list5.cpp
)
target_link_libraries(
    list5
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list5)