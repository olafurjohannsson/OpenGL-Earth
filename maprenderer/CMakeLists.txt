# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.16)

# Project's name
project(MapRenderer LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Qt's Meta-Object Compiler (moc)
set(CMAKE_AUTOMOC ON)

# Find the necessary Qt6 packages
find_package(Qt6 COMPONENTS Quick Core Qml REQUIRED)
# Find OpenGL
find_package(OpenGL REQUIRED)

set(SOURCES 
    Renderer.cpp
    Map.cpp
    lib/Coordinate.cpp
    lib/GeographicLoader.cpp
    lib/Polygon.cpp
    lib/Projection.cpp
)
# find proj library
find_library(PROJ_LIB NAMES proj)
if (NOT PROJ_LIB)
    message(FATAL_ERROR "PROJ library not found")
endif()
# Add shader files
# set(SHADER_FILES
#     shaders/vertex.glsl
#     shaders/fragment.glsl
# )

# qt6_add_resources(${PROJECT_NAME} "shaders"
#     PREFIX "/"
#     FILES
#         ${SHADER_FILES}
# )

# Define the library name
set(LIB_NAME MapRenderer)

# Add library files
add_library(${LIB_NAME} SHARED ${SOURCES})

# Specify include directories
target_include_directories(${LIB_NAME} PUBLIC include)

# link the libraries
target_link_libraries(${LIB_NAME} PRIVATE
    Qt6::Quick 
    Qt6::Core 
    Qt6::Qml 
    OpenGL::GL
    proj
)

qt6_add_resources(${PROJECT_NAME} "json"
    PREFIX "/"
    FILES
        "data/small.json"
)