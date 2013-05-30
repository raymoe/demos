cmake_minimum_required(VERSION 2.8)
project(demos)

option(DEBUG_mode "" ON)
message(${PROJECT_BINARY_DIR})
if(DEBUG_mode)
    message("debug")
    set(CMAKE_BUILD_TYPE debug)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/debug)
    #message(${EXECUTABLE_OUTPUT_PATH})
else()
    message("release")
    set(CMAKE_BUILD_TYPE release)
    #set(PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR}/bin/release)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/release)
endif()

#message(${CMAKE_C_FLAGS_DEBUG})
#set(CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG}-O0)
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
message(${CMAKE_C_FLAGS_DEBUG})

#This sentence must be put here in order to make the contents of the above effect
add_subdirectory(pthread)
add_subdirectory(net)




