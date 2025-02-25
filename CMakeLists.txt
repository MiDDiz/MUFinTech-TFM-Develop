cmake_minimum_required (VERSION 3.19)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (sqf)

# 1. Define some well-known folders:
# - Place for compile binaries
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
# - Root for include files
set(INCLUDE_HOME ${PROJECT_SOURCE_DIR}/src)
# - In-house cmake functions
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-lib)
# - set boost home
set(BOOST_ROOT /usr/local/lib ENV{BOOST_ROOT})


# Print some stuff
message("--> CMake extra functions: " ${CMAKE_MODULE_PATH})
message("--> Binaries: " ${CMAKE_BINARY_DIR})
message("--> Include base path: " ${INCLUDE_HOME})


# 2. Load in-house cmake modules
message("--> Loading CMake modules: ")
include(${CMAKE_MODULE_PATH}/CMakeBoostTest.cmake)
include(${CMAKE_MODULE_PATH}/CMakeAuxFunctions.cmake)

# Add GTest framework on compilation.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip # 1.16.0
)
FetchContent_MakeAvailable(googletest)

# 3. Set CMake environment
# - Set folders
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# - C++ file filter for adding files in cmake's projects
set(CXX_FILE_EXTENSIONS "*.h"  "*.cpp")

# 4. Set Compilation environment
include_directories(${INCLUDE_HOME})
add_definitions(-std=gnu++20)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# 5. Add subdirs
add_subdirectory(src)
