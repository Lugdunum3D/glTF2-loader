cmake_minimum_required(VERSION 3.1)

macro(lug_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# set android options
if(ANDROID)
    set(ANDROID_ABI arm64-v8a)
    message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")

    set(ANDROID_TOOLCHAIN clang)
    message(STATUS "ANDROID_TOOLCHAIN: ${ANDROID_TOOLCHAIN}")

    lug_set_option(ANDROID_PLATFORM android-24 STRING "Target specific android version target (default: android-24)")
    if(NOT ANDROID_PLATFORM MATCHES android-2[4-9])
        message(FATAL_ERROR "Invalid android version target: ${ANDROID_PLATFORM}")
    endif()
    message(STATUS "ANDROID_PLATFORM: ${ANDROID_PLATFORM}")

    lug_set_option(ANDROID_STL c++_shared STRING "Choose which Android STL to use (default: c++_shared)")

    if(NOT ANDROID_STL STREQUAL c++_static AND NOT ANDROID_STL STREQUAL c++_shared)
        message(FATAL_ERROR "Invalid STL: ${ANDROID_STL}")
    endif()
endif()

# project name
project(glTF2-loader C CXX)

# use c++14
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

# add the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

set(target "gltf2-loader")

set(SRCS
    "src/gltf2/glTF2.cpp"
)

set(INCS

)

add_library(${target} STATIC ${SRCS} ${INCS})

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

# unit tests
lug_set_option(BUILD_TESTS FALSE BOOL "TRUE to enable unit tests, FALSE to disable unit tests")
if(BUILD_TESTS)
    # Note: enable_testing() MUST be on the top level CMakeLists.txt
    enable_testing()
    add_subdirectory(test/)
endif()
