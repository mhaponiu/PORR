cmake_minimum_required(VERSION 3.6)
#cmake_minimum_required(VERSION 3.5.1)
project(PORR)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c wolf.c wolf.h directives.h maps.c maps.h)

add_executable(PORR ${SOURCE_FILES})

target_link_libraries(PORR m)
