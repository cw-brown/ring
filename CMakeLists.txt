cmake_minimum_required(VERSION 3.20...4.0)

set(PROJECT_NAME ring_container)
project(${PROJECT_NAME} LANGUAGES CXX)

option(BUILD_TESTS "Set to ON to build testing program" ON)

# Compiled settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3 -fconcepts-diagnostics-depth=2)
endif()

include(CTest)

# included libraries and headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# source files
# file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if(BUILD_TESTS)
    enable_testing()
endif()

add_executable(ConceptsAssert src/tests/concepts_assert.cpp)
add_test(NAME ConceptsAssert COMMAND ConceptsAssert)

add_executable(OperationsAssert src/tests/operations_test.cpp)
add_test(NAME OperationsAssert COMMAND OperationsAssert)
