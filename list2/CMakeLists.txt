add_executable(
    list2
    src/list2.cpp
)
target_link_libraries(
    list2
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list2)