########################################################################
# OceanRoute Nav — Maritime Navigation Optimizer
# CMake Build Configuration
# Authors: Shahzad Ahmad (24i-3090), Syed Muhammad Rayyan Hassan (24i-3000)
########################################################################
cmake_minimum_required(VERSION 3.16)
project(OceanRouteNav VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── SFML 2.6.1 Local Setup ──────────────────────────────────────────
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1")
set(SFML_INCLUDE_DIR "${SFML_DIR}/include")
set(SFML_LIB_DIR "${SFML_DIR}/lib")
set(SFML_BIN_DIR "${SFML_DIR}/bin")

# ── Source Files ─────────────────────────────────────────────────────
set(CORE_SOURCES
    src/core/DataStructures.h
    src/core/RouteData.h
    src/core/Constants.h
)

set(GRAPH_SOURCES
    src/graph/GraphNode.h
    src/graph/GraphNode.cpp
    src/graph/PortManagementSystem.h
    src/graph/PortManagementSystem.cpp
    src/graph/UserRouteList.h
    src/graph/UserRouteList.cpp
)

set(UI_SOURCES
    src/ui/Button.h
    src/ui/Button.cpp
    src/ui/Panel.h
    src/ui/Panel.cpp
    src/ui/StatusBar.h
    src/ui/StatusBar.cpp
    src/ui/TextInput.h
    src/ui/TextInput.cpp
)

set(SCENE_SOURCES
    src/scenes/Scene.h
    src/scenes/MainMenuScene.h
    src/scenes/MainMenuScene.cpp
    src/scenes/MapViewScene.h
    src/scenes/MapViewScene.cpp
    src/scenes/BookingScene.h
    src/scenes/BookingScene.cpp
    src/scenes/DijkstraScene.h
    src/scenes/DijkstraScene.cpp
    src/scenes/AStarScene.h
    src/scenes/AStarScene.cpp
    src/scenes/CustomShipScene.h
    src/scenes/CustomShipScene.cpp
    src/scenes/DockingScene.h
    src/scenes/DockingScene.cpp
    src/scenes/MultiLegScene.h
    src/scenes/MultiLegScene.cpp
)

set(RENDERING_SOURCES
    src/rendering/Renderer.h
    src/rendering/Renderer.cpp
    src/rendering/MapRenderer.h
    src/rendering/MapRenderer.cpp
)

set(SYSTEMS_SOURCES
    src/systems/AssetManager.h
    src/systems/AssetManager.cpp
    src/systems/SceneManager.h
    src/systems/SceneManager.cpp
)

set(USER_SOURCES
    src/user/User.h
    src/user/User.cpp
)

# ── Executable ───────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} WIN32
    src/main.cpp
    ${CORE_SOURCES}
    ${GRAPH_SOURCES}
    ${UI_SOURCES}
    ${SCENE_SOURCES}
    ${RENDERING_SOURCES}
    ${SYSTEMS_SOURCES}
    ${USER_SOURCES}
)

# ── Include Directories ─────────────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# ── Link SFML Libraries (Dynamic) ────────────────────────────────────
# For multi-config generators, use Release libs for all configs
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIB_DIR}/sfml-graphics.lib
    ${SFML_LIB_DIR}/sfml-window.lib
    ${SFML_LIB_DIR}/sfml-system.lib
    ${SFML_LIB_DIR}/sfml-audio.lib
    opengl32
    winmm
)

# ── Copy DLLs to output directory ───────────────────────────────────
file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
foreach(DLL ${SFML_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()

# ── Copy assets to output directory ─────────────────────────────────
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# ── Source Groups for Visual Studio ─────────────────────────────────
source_group("Core"      FILES ${CORE_SOURCES})
source_group("Graph"     FILES ${GRAPH_SOURCES})
source_group("UI"        FILES ${UI_SOURCES})
source_group("Scenes"    FILES ${SCENE_SOURCES})
source_group("Rendering" FILES ${RENDERING_SOURCES})
source_group("Systems"   FILES ${SYSTEMS_SOURCES})
source_group("User"      FILES ${USER_SOURCES})

# ── Compiler Options ────────────────────────────────────────────────
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /MP /utf-8)
    # Suppress common MSVC warnings for educational code
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
    )
    # Use main() entry point instead of WinMain
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# ── Output Configuration ────────────────────────────────────────────
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "========================================")
message(STATUS "  OceanRoute Nav v${PROJECT_VERSION}")
message(STATUS "  SFML: ${SFML_DIR}")
message(STATUS "  Build: ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
