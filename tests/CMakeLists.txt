# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Exit if not an OSX system
if (NOT CMAKE_HOST_APPLE)
	message(FATAL_ERROR "This repository only compiles on MacOS, CMake will exit.")
endif()

message("Running on Darwin ${CMAKE_HOST_SYSTEM_VERSION}")
if (CMAKE_HOST_SYSTEM_VERSION LESS 24)
	message(FATAL_ERROR "Your version is lesser than 15 - Sequoia. Cmake will exit.")
endif()

# Fetch Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

# Fetch Trompeloeil
FetchContent_Declare(
  Trompeloeil
  GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
  GIT_TAG        v49
)
FetchContent_MakeAvailable(Trompeloeil)

include(Catch)
include(CTest)
include(FetchContent)

#add_subdirectory(unitary)
add_subdirectory(integration)
