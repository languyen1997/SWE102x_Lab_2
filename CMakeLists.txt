cmake_minimum_required(VERSION 3.22)

project(SWE102x_LAB2
  LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_CXX_CFLAGS
  -Wall -Werror -Wextra -Wpedantic -Wold-style-cast
)

add_executable(AbstractFactory src/AbstractFactory.cpp)
target_compile_options(AbstractFactory PRIVATE ${PROJECT_CXX_CFLAGS})
set_target_properties(AbstractFactory PROPERTIES SUFFIX ".out")
