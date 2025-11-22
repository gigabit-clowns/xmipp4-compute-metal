# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.18)

# Define the project
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project(
  xmipp4-hardware-metal
	VERSION ${PROJECT_VERSION} 
	LANGUAGES C CXX
)

# Add the CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Include CMake packages
include(GNUInstallDirs)
include(CTest)
include(FetchContent)
include(fetch_metalcpp)

# Find dependencies
if(NOT TARGET xmipp4-core)
	find_package(xmipp4-core REQUIRED)
endif()

fetch_metalcpp(VERSION 26)

# Find all source and header files
file(
	GLOB_RECURSE 
	SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# Create the shared library
set(TARGET_NAME ${PROJECT_NAME})
add_library(${TARGET_NAME} SHARED ${SOURCES})
set_target_properties(
	${TARGET_NAME} 
	PROPERTIES 
		CXX_STANDARD 20
		DEFINE_SYMBOL "XMIPP4_HARDWARE_METAL_EXPORTING"
)
target_include_directories(
	${TARGET_NAME}
	PUBLIC
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

)
target_compile_definitions(
	${TARGET_NAME} 
	PRIVATE
		VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
		VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
		VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
)
target_link_libraries(
	${TARGET_NAME} 
	PRIVATE
		xmipp4-core
		metalcpp
)

# Set up clang-tidy
if(XMIPP4_HARDWARE_METAL_LINT_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)
	set_target_properties(
		${TARGET_NAME} 
		PROPERTIES 
			CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
	)
endif()

# Enable all warnings during compilation
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Install library's binary files and headers
install(
	TARGETS ${TARGET_NAME}
	EXPORT ${TARGET_NAME}-config
	LIBRARY DESTINATION ${XMIPP4_PLUGIN_INSTALL_DIR}
	RUNTIME DESTINATION ${XMIPP4_PLUGIN_INSTALL_DIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
	EXPORT ${TARGET_NAME}-config
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
)

# Only build tests if it is the main project
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
