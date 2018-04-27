cmake_minimum_required (VERSION 3.5)

project (ex17)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99")
set (source_dir "${PROJECT_SOURCE_DIR}")

file (GLOB source_files "${source_dir}/*.c")

add_executable (ex17 ${source_files})
