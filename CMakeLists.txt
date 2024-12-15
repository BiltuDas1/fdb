cmake_minimum_required(VERSION 3.29.3)
project(fdb LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(xxh3 SHARED xxHash/xxhash.c)
target_include_directories(xxh3 PUBLIC xxHash)

add_library(core SHARED include/core/core.cpp)
target_include_directories(core PUBLIC include/core)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE xxh3 core rocksdb uv)
target_include_directories(${PROJECT_NAME} PRIVATE include/depends)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wpedantic)
set_target_properties(${PROJECT_NAME} PROPERTIES
  BUILD_RPATH "\$ORIGIN"
)