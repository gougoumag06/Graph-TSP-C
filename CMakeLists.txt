cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME graph)
project(${NAME} VERSION 1.0)

option(VS_DEBUG_RELEASE "Generate only DEBUG and RELEASE configuration on VS" ON)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    link_libraries(OpenMP::OpenMP_C)
endif()


message(STATUS "[INFO] Current directory: " ${CMAKE_SOURCE_DIR})

if(MSVC AND VS_DEBUG_RELEASE)
    message(STATUS "[INFO] Configurations: DEBUG; RELEASE")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif()

if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "[INFO] Multi-configuration generator")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_bin/")
else()
    message(STATUS "[INFO] Single-configuration generator")
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_bin/${CMAKE_BUILD_TYPE}")
endif()


#-------------------------------------------------------------------------------
# Projects

add_subdirectory(application)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT application)
endif()
