## \brief     A Qt based GUI for the TLS Pool
## \author    Aschwin Marsman
## \date      2016
## \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
## \license   SPDX-License-Identifier: GPL-2.0

## Require a minimum version of CMake
message(FATAL_ERROR "This repository has moved to GitLab\n\nGet the project from\n\thttps://gitlab.com/arpa2/tlspool-gui/\n")
cmake_minimum_required(VERSION 3.1.0)

## Application name
set(APP_NAME "tlspool-gui")

## Project name
project(${APP_NAME})

## For now hard coded:
SET(CMAKE_BUILD_TYPE Debug)

## \todo check should be for g++ or clang.
if (APPLE)
	set(COMPILE_FLAGS "-Weverything -Wno-padded")
else()
        set(COMPILE_FLAGS "-Wall -Wextra -std=c++11 -Wshadow -Wold-style-cast -Wfloat-equal -Woverloaded-virtual -Wundef -DDEBUG")
endif ()
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

#set(CMAKE_CXX_STANDARD 14)

## Include (or not) the full compiler output
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Find the Qt libraries
find_package(Qt5Svg REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(${Qt5Svg_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})

# \todo use something like find_package?
include_directories(../tlspool/include)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Instruct CMake to run ccc automatically when needed.
set(CMAKE_AUTORCC ON)

## Sources
file(GLOB MAIN_SRC src/*.cpp)
set(SOURCES ${MAIN_SRC})

## Executable
add_executable(${APP_NAME} ${SOURCES} src/resources.qrc)

# Use the Widgets module from Qt 5.
target_link_libraries(${APP_NAME} Qt5::Widgets Qt5::Svg tlspool)

add_custom_target(
    GenerateHeaderFile
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generatefiles.sh ${CMAKE_CURRENT_SOURCE_DIR}/src/tlspoolgenerated.h
#    COMMAND /home/aschwin/projects/arpa2/amarsman/tlspool/2016-07-14-tlspool-environment/tlspool-gui/scripts/generateversion.sh /home/aschwin/projects/arpa2/amarsman/tlspool/2016-07-14-tlspool-environment/tlspool-gui/src/tlspoolversion.h
)
add_dependencies(${APP_NAME} GenerateHeaderFile)

set(CMAKE_INSTALL_PREFIX "")

install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION bin)
