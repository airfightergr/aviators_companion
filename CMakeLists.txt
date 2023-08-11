cmake_minimum_required(VERSION 3.26)
project(aviators_companion )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# cimgui stuff
#if (EXISTS ${CMAKE_INCLUDE_CURRENT_DIR}cimgui/imgui/backends)
    set(BACKENDS_FOLDER "cimgui/imgui/backends/")
#else ()
#    set(BACKENDS_FOLDER "cimgui/imgui/examples/")
#endif ()

if (EXISTS ${CMAKE_INCLUDE_CURRENT_DIR}/cimgui/imgui/imgui_tables.cpp)
    set(TABLES_SOURCES "cimgui/imgui/imgui_tables.cpp")
else ()
    set(TABLES_SOURCES "")
endif ()

include_directories(cimgui/imgui)
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(cimgui)
set(IMGUI_SOURCES
        cimgui/cimgui.cpp
        cimgui/imgui/imgui.cpp
        cimgui/imgui/imgui_draw.cpp
        cimgui/imgui/imgui_demo.cpp
        cimgui/imgui/imgui_widgets.cpp
        ${TABLES_SOURCES}
)

set(IMGUI_SOURCES_sdl)
set(IMGUI_LIBRARIES)

if (WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

# optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
    FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
    list(APPEND IMGUI_LIBRARIES freetype)
    list(APPEND IMGUI_SOURCES cimgui/imgui/misc/freetype/imgui_freetype.cpp)
    add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

# opengl3
list(APPEND IMGUI_SOURCES ${BACKENDS_FOLDER}imgui_impl_opengl3.cpp)
include_directories(cimgui/imgui/examples/libs/gl3w)
if(WIN32)
    list(APPEND IMGUI_LIBRARIES opengl32)
else(WIN32) # Unix
    list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)

# GLFW
list(APPEND IMGUI_SOURCES ${BACKENDS_FOLDER}imgui_impl_glfw.cpp)

set(GLFW_VERSION 3.3.8)
include(FetchContent)
FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz)

FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# glfw/imgui gets confused if it is not statically built.
IF (WIN32)
    add_library(cimgui STATIC ${IMGUI_SOURCES})
ELSE()
    add_library(cimgui SHARED ${IMGUI_SOURCES})
ENDIF()

target_link_libraries(cimgui ${IMGUI_LIBRARIES} glfw)

include_directories(cimgui/generator/output/)


add_executable(aviators_companion src/main.c
        src/conversions.h
        src/conversions.c
        src/cimgui_main.c
        src/cimgui_main.h
        src/main.h)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")

set_target_properties(aviators_companion PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW)
if (MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE "-mconsole")
endif()
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cimgui/cimgui.so cimgui)
