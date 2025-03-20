#***************************************************************************
# Authors:     Mikel Iceta Tena (miceta@cnb.csic.es)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA
#
#  All comments concerning this program package may be sent to the
#  e-mail address 'xmipp@cnb.csic.es'
# ***************************************************************************

cmake_minimum_required(VERSION 3.18)

# Exit if not an OSX system
if (NOT CMAKE_HOST_APPLE)
	message(FATAL_ERROR "This repository only compiles on MacOS, CMake will exit.")
endif()

# Exit if OS too old for Metal-CPP
# Oldest supported version is MacOS12
message("Running on Darwin ${CMAKE_HOST_SYSTEM_VERSION}")
if (CMAKE_HOST_SYSTEM_VERSION LESS 21)
	message(FATAL_ERROR "Your version is lesser than 12 - Monterey. Cmake will exit.")
endif()

# Define the project
project(
  xmipp4-compute-metal
	VERSION 0.1.0 
	LANGUAGES C CXX
)

# Include CMake packages
include(GNUInstallDirs)
include(CTest)
include(FetchContent)

# Declare build options
option(XMIPP4_COMPUTE_METAL_LINT_CLANG_TIDY "Lint with clang-tidy" OFF)

# Find xmipp4-core
if(NOT TARGET xmipp4-core)
	find_package(xmipp4-core REQUIRED)
endif()

# Get MacOS version
execute_process(
    COMMAND sw_vers -productVersion
    OUTPUT_VARIABLE MACOS_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Extract major and minor version
string(REPLACE "." ";" VERSION_LIST ${MACOS_VERSION})
list(GET VERSION_LIST 0 MACOS_MAJOR)
list(GET VERSION_LIST 1 MACOS_MINOR)

# Construct the branch name based on MacOS version
set(METAL_CPP_BRANCH "macos-${MACOS_MAJOR}.${MACOS_MINOR}")

# Download METAL-CPP dependencies
set(METAL_CPP_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/metal-cpp")
FetchContent_Declare(
	"METAL_CPP"
	GIT_REPOSITORY "https://github.com/bkaradzic/metal-cpp.git"
	GIT_TAG "metal-cpp_macOS12_iOS15"
	SOURCE_DIR "${METAL_CPP_SOURCE_DIR}"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)
FetchContent_MakeAvailable("METAL_CPP")

add_library(METAL_CPP INTERFACE IMPORTED)
target_include_directories(METAL_CPP INTERFACE ${METAL_CPP_SOURCE_DIR})
target_link_libraries(METAL_CPP INTERFACE
         "-framework Metal"
         "-framework MetalKit"
         "-framework Foundation"
)

# Find all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Create the shared library
add_library(${PROJECT_NAME} MODULE ${SOURCES})
set_target_properties(
	${PROJECT_NAME} 
	PROPERTIES 
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_COMPUTE_METAL_EXPORTING"
)
target_compile_definitions(
	${PROJECT_NAME} 
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)
target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE
		xmipp4-core
		METAL_CPP
)

# Set up clang-tidy
if(XMIPP4_COMPUTE_METAL_LINT_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)
	set_target_properties(
		${PROJECT_NAME} 
		PROPERTIES 
			CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
	)
endif()

# Enable all warnings during compilation
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Disable min/max macros in windows
if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DNOMINMAX)
endif()

# Install library's binary files and headers
install(
	TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/xmipp4/plugins/
)

# Only build tests if it is the main project
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
