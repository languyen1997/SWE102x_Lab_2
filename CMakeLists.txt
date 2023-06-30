cmake_minimum_required(VERSION 3.22)

project(SWE102x_LAB2
  LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_CXX_CFLAGS
  -Wall -Werror -Wextra -Wpedantic -Wold-style-cast
)

list(APPEND Patterns
  # Creational Patterns
  AbstractFactory
  Builder
  FactoryMethod
  Prototype
  Singleton
  # Structural Patterns
  Adapter
  Bridge
  Composite
  Decorator
  Facade
  Flyweight
)

foreach(pattern IN LISTS Patterns)
  add_executable(${pattern} src/${pattern}.cpp)
  target_compile_options(${pattern} PRIVATE ${PROJECT_CXX_CFLAGS})
  set_target_properties(${pattern} PROPERTIES SUFFIX ".out")
endforeach()
