# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(take_picture)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pthread -std=c++11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Concurrent REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

message(STATUS "Checking PYLON_ROOT:" $ENV{PYLON_ROOT})
if(DEFINED ENV{PYLON_ROOT})
    message(STATUS "PYLON_ROOT environment variable defined")
else()
    message(FATAL_ERROR "PYLON_ROOT environment variable not defined")
endif()

link_directories($ENV{PYLON_ROOT}/lib64)
set(PYLON_LIBRARIES
    pylonbase
    pylonutility
    GenApi_gcc_v3_0_Basler_pylon_v5_0
    GCBase_gcc_v3_0_Basler_pylon_v5_0)

include_directories(
    ${CMAKE_SOURCE_DIR}
    SYSTEM $ENV{PYLON_ROOT}/include
    ${OpenCV_INCLUDE_DIRS})

# Find necessary packages for python wrapper
find_package(PythonLibs 3.5 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})

set(Boost_USE_STATIC_LIBS   OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS python-py35 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Supports only running python stuff in
# development/build environment
# TODO move to CMAKE_INSTALL_PREFIX/.. ??
set(PYTHON_SCRIPTS_PATH ${CMAKE_BINARY_DIR})
configure_file(config.h.in config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})

# Core libtakepictures sources
set(CORE_SOURCES
    cvcapture.cpp)

set(CORE_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    ${OpenCV_LIBS}
    ${PYLON_LIBRARIES})

# Python libpoctoyota sources/libs
set(PYTHON_SOURCES
    ${SOURCES}
    python_module.cpp
    mat_converter_cv3.cpp
    )
set(PYTHON_LIBS
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES})

# Declare the takePicture library
add_library(takepicture SHARED
    ${CORE_SOURCES}
    ${PYTHON_SOURCES})
target_link_libraries(takepicture
    ${CORE_LIBS}
    ${PYTHON_LIBS})

target_include_directories(takepicture PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}")

# FIXME workaround to a build issue in Boost.python.numpy
set_target_properties(takepicture PROPERTIES COMPILE_FLAGS "-fpermissive")

install(FILES ${PYTHON_SOURCES} DESTINATION ${CMAKE_BINARY_DIR})
