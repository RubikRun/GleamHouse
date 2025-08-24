cmake_minimum_required(VERSION 3.6...3.31)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set avaiable configurations to Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Create project GleamHouse
project(GleamHouse)

option(GLEAMHOUSE_WITH_DEBUG_GRAPHICS "Enable debug graphics in Gleam House" OFF)

# Add Pekan subdirectory, without demo projects
set(WITH_DEMO_PROJECTS OFF)
add_subdirectory(dep/Pekan)
set_target_properties(Core PROPERTIES FOLDER "dep/Pekan")
set_target_properties(Graphics PROPERTIES FOLDER "dep/Pekan")
set_target_properties(Renderer2D PROPERTIES FOLDER "dep/Pekan")
set_target_properties(GUI PROPERTIES FOLDER "dep/Pekan")
set_target_properties(Tools PROPERTIES FOLDER "dep/Pekan")


# Add an executable GleamHouse, compiling the following source files
add_executable(GleamHouse
    src/main.cpp
    src/GleamHouse_Application.h
    src/GleamHouse_Application.cpp
    src/GleamHouse_Scene.h
    src/GleamHouse_Scene.cpp
    src/Floor.h
    src/Floor.cpp
    src/Player.h
    src/Player.cpp
)

# Set link libraries for GleamHouse
target_link_libraries(GleamHouse PRIVATE
    Core
    Renderer2D
    GUI
    Tools
)

target_compile_definitions(GleamHouse PRIVATE
    # Set GLEAMHOUSE_ROOT_DIR definition to be the path to current source directory
    GLEAMHOUSE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    # Set GLEAMHOUSE_WITH_DEBUG_GRAPHICS definition to be 0 or 1 depending on the on/off state of the option
    GLEAMHOUSE_WITH_DEBUG_GRAPHICS=$<IF:$<BOOL:${GLEAMHOUSE_WITH_DEBUG_GRAPHICS}>,1,0>
)

# Set GleamHouse to be the startup project by default
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GleamHouse)

# Set GleamHouse's working directory to be GleamHouse's source directory
# so that we can write filepaths of resources relative to GleamHouse's source directory.
set_target_properties(GleamHouse PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
