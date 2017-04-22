cmake_minimum_required(VERSION 3.1)

# project name
project(test)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

# Find gtest
find_package(GTest)

include_directories(${GTEST_INCLUDE_DIRS})

find_package(Threads REQUIRED)

if(NOT DEFINED TEST_OUTPUT)
    set(TEST_OUTPUT ".")
endif()

set(target runUnitTests)
set(SRC
    ${PROJECT_SOURCE_DIR}/basic.cpp
)
source_group("src" FILES ${SRC})

add_executable(${target} ${SRC} ${PROJECT_SOURCE_DIR}/main.cpp)

# c++14 on android (CMAKE_CXX_STANDARD doesn't work with the Android Toolchain)
if (ANDROID)
    target_compile_options(${target} PUBLIC -std=c++14)
endif()

# use warnings and errors
if (MSVC)
    target_compile_options(${target} PUBLIC /W4 /WX)
else()
    target_compile_options(${target} PUBLIC -Wall -Wextra -Werror)
endif()

# add parallel build for MSVC
if (MSVC)
    target_compile_options(${target} PUBLIC /MP)
endif()

target_link_libraries(${target} gltf2-loader ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME ${name}UnitTests COMMAND ${target} --gtest_output=xml:${TEST_OUTPUT}/${name}UnitTests.xml)
