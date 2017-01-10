cmake_minimum_required(VERSION 3.6)
#cmake_minimum_required(VERSION 3.5.1)
project(PORR)

set(CMAKE_C_STANDARD 11)

#add_definitions(-DCOUNT_WOLVES=300)
#add_definitions(-DITER=10000)
#add_definitions(-DDEBUG)
#add_definitions(-DGENERATOR=1)
#add_definitions(-DTIME)

set(SOURCE_FILES main.c wolf.c wolf.h directives.h maps.c maps.h)

add_executable(PORR ${SOURCE_FILES})

target_link_libraries(PORR m)
