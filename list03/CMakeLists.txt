add_executable(
    list3
    src/list3.cpp
)
target_link_libraries(
    list3
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list3)