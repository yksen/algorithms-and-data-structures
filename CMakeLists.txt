cmake_minimum_required(VERSION 3.14)
project(algorithms-and-data-structures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(list1)
add_subdirectory(list2)
add_subdirectory(list3)
add_subdirectory(list4)
add_subdirectory(list5)
add_subdirectory(list6)