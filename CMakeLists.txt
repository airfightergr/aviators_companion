cmake_minimum_required(VERSION 3.22)
project(aviators_companion CXX)

set(CMAKE_CXX_STANDARD 11)

set(IMGUI_DIR imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/baskends)

find_package(OpenGL REQUIRED)

set(LIBRARIES "glfw;OpenGL")

file(GLOB sources src/main.cpp src/main.h src/imgui_main.cpp src/imgui_main.h src/fuel.cpp src/fuel.h src/conversions.cpp src/conversions.h)

add_executable(aviators_companion
                ${sources}
                ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
                ${IMGUI_DIR}/imgui.cpp
                ${IMGUI_DIR}/imgui_draw.cpp
                ${IMGUI_DIR}/imgui_demo.cpp
                ${IMGUI_DIR}/imgui_tables.cpp
                ${IMGUI_DIR}/imgui_widgets.cpp
)

target_link_libraries(aviators_companion ${LIBRARIES})
target_compile_definitions(aviators_companion PUBLIC -DImTextureID=ImU64 )

#
#if (EXISTS ${CMAKE_INCLUDE_CURRENT_DIR}/imgui/imgui_tables.cpp)
#    set(TABLES_SOURCES "imgui/imgui_tables.cpp")
#else ()
#    set(TABLES_SOURCES "")
#endif ()
#
#add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")
#
#
#
#set(IMGUI_SOURCES
#        imgui/imgui.cpp
#        imgui/imgui_draw.cpp
#        imgui/imgui_demo.cpp
#        imgui/imgui_widgets.cpp
#        imgui/imgui_tables.cpp
#        ${TABLES_SOURCES}
#)
#
#set(IMGUI_SOURCES_sdl)
#set(IMGUI_LIBRARIES)
#
#if (WIN32)
#    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
#else(WIN32)
#    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
#endif(WIN32)
#
#add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")
#
## optional adding freetype
#option(IMGUI_FREETYPE "add Freetype2" OFF)
#
#if(IMGUI_FREETYPE)
#    FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
#    list(APPEND IMGUI_LIBRARIES freetype)
#    list(APPEND IMGUI_SOURCES imgui/misc/freetype/imgui_freetype.cpp)
#    add_definitions("-DCIMGUI_FREETYPE=1")
#endif(IMGUI_FREETYPE)
#
#
#if(WIN32)
#    list(APPEND IMGUI_LIBRARIES opengl32)
#else(WIN32) # Unix
#    list(APPEND IMGUI_LIBRARIES GL)
#endif(WIN32)
#
## GLFW
#list(APPEND IMGUI_SOURCES ${BACKENDS_FOLDER}/imgui_impl_glfw.cpp)
#
#set(GLFW_VERSION 3.3.8)
##include(FetchContent)
##FetchContent_Declare(
##        glfw
##        URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz)
##
##FetchContent_GetProperties(glfw)
##if (NOT glfw_POPULATED)
##    set(FETCHCONTENT_QUIET NO)
###    FetchContent_Populate(glfw)
##    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
##    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
###    add_subdirectory("${glfw_SOURCE_DIR}/${glfw_BINARY_DIR}")
##endif()
#
## glfw/imgui gets confused if it is not statically built.
##IF (WIN32)
##    add_library(imgui STATIC ${IMGUI_SOURCES})
##ELSE()
##    add_library(imgui SHARED ${IMGUI_SOURCES})
##ENDIF()
##
##target_link_libraries(imgui ${IMGUI_LIBRARIES} glfw)
##
#
#
#add_executable(aviators_companion
#        src/main.cpp            src/main.h
#        src/conversions.cpp     src/conversions.h
#        src/fuel.cpp            src/fuel.h
#        src/imgui_main.cpp      src/imgui_main.h
#)
#
#set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
#
#set_target_properties(aviators_companion PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DIMGUI_USE_OPENGL3 -DIMGUI_USE_GLFW )
#if (MINGW)
#    target_link_options(${PROJECT_NAME} PRIVATE "-mconsole")
#endif()

#target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libimgui.so imgui)
