cmake_minimum_required(VERSION 3.20)
project(Kaizen VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SFML (SFML 3 uses different component names)
find_package(SFML 3 COMPONENTS Graphics Window Audio REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files explicitly (avoid globs to prevent accidental additions)
set(SOURCES
    src/Animation.cpp
    src/Attack.cpp
    src/Boss.cpp
    src/Camera.cpp
    src/EffectsManager.cpp
    src/Enemy.cpp
    src/Entity.cpp
    src/Game.cpp
    src/HUD.cpp
    src/main.cpp
    src/ParticleSystem.cpp
    src/Physics.cpp
    src/PixelArtGenerator.cpp
    src/Platform.cpp
    src/Player.cpp
    src/ResourceManager.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML (SFML 3 library names)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::Audio
)

# Compiler warnings (best practice)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Copy SFML DLLs to build directory (Windows only)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
