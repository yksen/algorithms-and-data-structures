add_executable(
    list9
    src/list9.cpp
)
add_executable(
    progdyn1
    src/progdyn1.cc
)
add_executable(
    progdyn2
    src/progdyn2.cc
)
target_link_libraries(
    list9
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(list9)