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

# Include CMake packages
include(GNUInstallDirs)
include(CTest)
include(FetchContent)

# Declare build options
option(XMIPP4_HARDWARE_METAL_LINT_CLANG_TIDY "Lint with clang-tidy" OFF)

# Find xmipp4-core
if(NOT TARGET xmipp4-core)
	find_package(xmipp4-core REQUIRED)
endif()

# Download METAL-CPP dependencies
set(METAL_CPP_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/metal-cpp")
FetchContent_Declare(
	"METAL_CPP"
	GIT_REPOSITORY "https://github.com/bkaradzic/metal-cpp.git"
	GIT_TAG "metal-cpp_macOS15.2_iOS18.2"
	SOURCE_DIR "${METAL_CPP_SOURCE_DIR}"
)
FetchContent_MakeAvailable("METAL_CPP")

set(TARGET_NAME ${PROJECT_NAME})

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
		METAL_CPP
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
