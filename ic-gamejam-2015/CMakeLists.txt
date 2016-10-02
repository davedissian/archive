cmake_minimum_required(VERSION 2.8)

project (ICGameJam15)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(SRC
    src/Bullet.cpp
    src/Cursor.cpp
    src/Explosion.cpp
    src/Game.cpp
    src/Main.cpp
    src/Map.cpp
    src/Track.cpp
    src/Train.cpp
    src/Turret.cpp
    src/Waypoint.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/dependencies/SFML")
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

message(STATUS ${SFML_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
add_executable(gjam ${SRC})
target_link_libraries(gjam ${SFML_LIBRARIES})
