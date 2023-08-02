cmake_minimum_required(VERSION 3.26)
project(aviators_companion C)

set(CMAKE_C_STANDARD 11)

add_executable(aviators_companion src/main.c
        src/conversions.h
        src/conversions.c)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")

set_target_properties(aviators_companion PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
