# Posessed - Entry to IC GameJam 2014
# Copyright (c) 2014 David Avedissian (avedissian.david@gmail.com)

cmake_minimum_required(VERSION 2.8)
project(GameJam14)

# Include modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

# Set default build type
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Set install location
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
endforeach()

# Enable C++11
add_definitions(-std=c++11)

# Dependencies
set(SFML_ROOT "/vol/bitbucket/da913/public/SFML/")
set(BOX2D_DIR "/vol/bitbucket/da913/public/Box2D/")
find_package(SFML REQUIRED COMPONENTS system graphics window)
find_package(Box2D REQUIRED)

# Files
set (SRCS
  src/main.cpp
  src/object.cpp
  src/entity.cpp
  src/states.cpp
  src/trigger.cpp
  src/level.cpp
  src/timer.cpp)

# Add include directories and library paths for dependencies
include_directories(${SFML_INCLUDE_DIR} ${BOX2D_INCLUDE_DIR})
set(LIBS ${SFML_LIBRARIES} ${BOX2D_LIBRARIES})

# Add the executable
add_executable(posessed ${SRCS} ${HDRS})
target_link_libraries(posessed ${LIBS})
