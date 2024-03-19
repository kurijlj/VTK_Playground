# ============================================================================
#
# A CMake build script for the C++ VTK Playground project
#
# ============================================================================


# ============================================================================
#
# References
#
# ============================================================================
#
# For more information about using CMake read the documentation on:
#   * https://cmake.org/cmake/help/latest/guide/tutorial/index.html
#   * https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
#   * https://cmake.org/examples/
#
# ============================================================================


# ----------------------------------------------------------------------------
# Set the minimum required version of CMake
# ----------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.15...3.40)


# ----------------------------------------------------------------------------
# Set the project name and version
# ----------------------------------------------------------------------------

project (
    VTK_Playground
    VERSION 0.1
    DESCRIPTION "VTK self-study project"
    LANGUAGES CXX
    )


# ----------------------------------------------------------------------------
# Set the C++ standard
# ----------------------------------------------------------------------------

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)


# ----------------------------------------------------------------------------
# Set CMake module path
# ----------------------------------------------------------------------------

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# ----------------------------------------------------------------------------
# Set the project options
# ----------------------------------------------------------------------------

# Set the build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
endif ()

# By default, we want to statically link to the libraries, but if we are
# building in Windows, we want to build shared libraries
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (BUILD_SHARED_LIBS ON)  # Make sure we build shared libraries on Windows
elseif (NOT DEFINED BUILD_SHARED_LIBS)
    set (BUILD_SHARED_LIBS OFF)  # Build static libraries by default on other
endif ()

# Determine whether the libraries are built as shared or static
if (BUILD_SHARED_LIBS)
    set (LIB_TYPE SHARED)
else ()
    set (LIB_TYPE STATIC)
endif (BUILD_SHARED_LIBS)


# ----------------------------------------------------------------------------
# Set the output directory for the executable
# ----------------------------------------------------------------------------

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# ----------------------------------------------------------------------------
# Add external libraries
# ----------------------------------------------------------------------------

# Add the clipp.h library
add_library (clipp INTERFACE)
target_sources (clipp INTERFACE "${PROJECT_SOURCE_DIR}/extern/clipp/clipp.hpp")
target_include_directories (
    clipp
    INTERFACE
    "${PROJECT_SOURCE_DIR}/extern/clipp"
    )


# ----------------------------------------------------------------------------
# Search for the required packages
# ----------------------------------------------------------------------------

# Find the DirectX components
find_package (DirectX REQUIRED)
if (NOT DirectX_FOUND)
  message("Terminating configuration: ${DirectX_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Find the OpenGL components
find_package(Qt5 COMPONENTS Widgets REQUIRED)
if (NOT Qt5Widgets_FOUND)
  message("Terminating configuration: ${Qt5Widgets_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Find the VTK components
find_package(VTK
  COMPONENTS
    CommonCore
    GUISupportQt
    IOImage
    ImagingColor
    ImagingGeneral
    InteractionImage
    InteractionStyle
    InteractionWidgets)
if (NOT VTK_FOUND)
  message("Terminating configuration: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# ----------------------------------------------------------------------------
# Add the source code
# ----------------------------------------------------------------------------

add_subdirectory (src)