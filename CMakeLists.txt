cmake_minimum_required(VERSION 3.16)

# Set your project name
project(Map LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the necessary Qt6 packages
find_package(Qt6 COMPONENTS Quick Core Qml REQUIRED)
# Find OpenGL
find_package(OpenGL REQUIRED)

#find_package(MapRenderer REQUIRED)

# Add QML files
set(QML_FILES
    qml/main.qml 
)

# Add all your .cpp files here
set(SOURCES
    main.cpp
)

# Add the executable
qt_add_executable(${PROJECT_NAME} ${SOURCES})

# add directory of library
add_subdirectory(maprenderer)

# So we can do #include <headers>
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/maprenderer)

# Link the executable to the necessary libraries
target_link_libraries(${PROJECT_NAME}  PRIVATE
    Qt6::Quick 
    Qt6::Core 
    Qt6::Qml 
    OpenGL::GL
    MapRenderer
    )



qt6_add_resources(${PROJECT_NAME} "qml"
    PREFIX "/"
    FILES
        ${QML_FILES}
)