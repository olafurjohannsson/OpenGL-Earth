# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.16)

# Project's name
project(MapRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the necessary Qt6 packages
find_package(Qt6 COMPONENTS Quick Core Qml REQUIRED)
# Find OpenGL
find_package(OpenGL REQUIRED)

# Add library files
add_library(MapRenderer SHARED Renderer.cpp Map.cpp)

# Specify include directories
target_include_directories(MapRenderer PUBLIC include)

# link the libraries
target_link_libraries(${PROJECT_NAME}  PRIVATE
    Qt6::Quick 
    Qt6::Core 
    Qt6::Qml 
    OpenGL::GL
)

