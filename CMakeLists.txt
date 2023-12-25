cmake_minimum_required(VERSION 3.6)
project(projects-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add third-party libraries
add_subdirectory(third_party/gtest)

add_subdirectory(coding_problems)
add_subdirectory(multithreading_interrupt)
add_subdirectory(multithreading_odd_even_print)
add_subdirectory(thread_inherritance)

add_subdirectory(public_types)
