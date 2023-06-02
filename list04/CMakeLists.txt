add_executable(
    list4
    src/list4.cpp
)
add_executable(
    sorty2023
    src/sorty2023.cc
)
target_link_libraries(
    list4
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list4)