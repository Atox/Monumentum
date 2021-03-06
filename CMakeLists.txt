# Monumentum toolkit

cmake_minimum_required(VERSION 3.4)
project(Monumentum)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g")
endif()
# MSVC FIX
if(MSVC)
    add_definitions(-D_ENABLE_ATOMIC_ALIGNMENT_FIX)
endif()
# Source files and includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE AUTO_SOURCES "Monumentum/*")
# MSVC additional directories
if(MSVC)
    set(INCLUDE_DIRS "" CACHE STRING "Additional include directories")
    set(LIBRARIES_DIRS "" CACHE STRING "Location of libraries")
    if(NOT INCLUDE_DIRS STREQUAL "")
        include_directories(${INCLUDE_DIRS})
    endif()
    if(NOT LIBRARIES_DIRS STREQUAL "")
        link_directories(${LIBRARIES_DIRS})
    endif()
endif()
# Static library
add_library(monumentum_static STATIC ${AUTO_SOURCES})
if(WIN32)
    set_target_properties(monumentum_static PROPERTIES OUTPUT_NAME Monumentum_s)
    add_definitions(-DNOMINMAX)
else()
    set_target_properties(monumentum_static PROPERTIES OUTPUT_NAME Monumentum)
endif()
# Shared library
add_library(monumentum_shared SHARED ${AUTO_SOURCES})
set_target_properties(monumentum_shared PROPERTIES OUTPUT_NAME Monumentum)
if(MSVC)
    target_link_libraries(monumentum_shared ws2_32.lib winmm.lib)
endif()
# Link
if(MSVC)
    target_link_libraries(monumentum_static general enet general liblz4_static)
    target_link_libraries(monumentum_shared general enet general liblz4)
else()
    target_link_libraries(monumentum_static enet lz4)
endif()
if(UNIX)
    INSTALL (TARGETS monumentum_shared DESTINATION "/usr/lib")
endif()
