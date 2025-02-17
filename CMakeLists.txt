# Only tested on windows/mvsc

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there.
mkdir build && cd build && cmake ..")
endif()

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Solitaire VERSION 1.0)

# Source files
add_executable(
    ${PROJECT_NAME}

    # Main file
    src/Source.cpp

    src/Board.cpp
    src/BoardPiecesMenuState.cpp
    src/BoardSHapeMenuState.cpp
    src/BoardSizeMenuState.cpp
    src/Button.cpp
    src/Errors.cpp
    src/GameEngine.cpp
    src/GameState.cpp
    src/InputManager.cpp
    src/InstructionsState.cpp
    src/LTexture.cpp
    src/MenuState.cpp
    src/PlayGameState.cpp
    src/SettingsState.cpp
    src/Slider.cpp
)

# Project include
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
# Include files
target_include_directories(${PROJECT_NAME} PUBLIC deps/include)

# Libs
find_library(SDL2 SDL2.lib REQUIRED PATHS deps/lib)
find_library(SDL2M SDL2main.lib REQUIRED PATHS deps/lib)
find_library(IMAGE SDL2_image.lib REQUIRED PATHS deps/lib)
find_library(TTF SDL2_ttf.lib REQUIRED PATHS deps/lib)
find_library(MIXER SDL2.lib REQUIRED PATHS deps/lib)

target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2} ${SDL2M} ${IMAGE} ${TTF} ${MIXER})

# Copy DLLs into the correct folder (either Debug or Release) for Ninja builds
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/deps/dll 
                   ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Copy DLLs in build folder for non-IDE builds
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/deps/dll 
                   ${CMAKE_BINARY_DIR})

# Copy fonts folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/fonts ${CMAKE_BINARY_DIR}/fonts)

# Copy textures folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures)

# Set the PATH to include the DLL directory for runtime (for IDE like Visual Studio)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_ENVIRONMENT "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>;$ENV{PATH}"
)
