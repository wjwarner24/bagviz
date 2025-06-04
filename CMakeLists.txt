cmake_minimum_required(VERSION 3.16)
project(bagviz)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenSceneGraph REQUIRED COMPONENTS osg osgDB osgViewer)
find_package(SQLite3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Enable Qt's MOC, UIC, and RCC automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Collect all source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.hpp
)

# Create executable
add_executable(bagviz ${SRC_FILES})

# Link libraries
target_link_libraries(bagviz
    Qt5::Widgets
    osg
    osgDB
    osgViewer
    SQLite::SQLite3
    yaml-cpp
)
