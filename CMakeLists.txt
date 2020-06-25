cmake_minimum_required(VERSION 3.14)
project(number_of_square_free_words)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -Werror -Wshadow -pedantic)

add_executable(number_of_square_free_words main.cpp)