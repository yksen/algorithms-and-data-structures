add_executable(
    list1
    src/list1.cpp
)
target_link_libraries(
    list1
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list1)