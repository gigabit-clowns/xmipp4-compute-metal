cmake_minimum_required(VERSION 3.12)

include(FetchContent)

function(fetch_metalcpp)
	set(options)
	set(oneValueArgs VERSION)
	set(multiValueArgs)
	cmake_parse_arguments(PARSE_ARGV 0 arg
		"${options}" "${oneValueArgs}" "${multiValueArgs}"
	)

	cmake_policy(SET CMP0135 NEW) # To avoid warnings
	set(NO_OP_COMMAND "${CMAKE_COMMAND} -E true")
	FetchContent_Declare(
		metalcpp
        URL https://developer.apple.com/metal/cpp/files/metal-cpp_${arg_VERSION}.zip
		CONFIGURE_COMMAND ${NO_OP_COMMAND}
		BUILD_COMMAND ${NO_OP_COMMAND}
		INSTALL_COMMAND ${NO_OP_COMMAND}
	)
	FetchContent_MakeAvailable(metalcpp)

    set(metalcpp_INCLUDE_DIR ${metalcpp_SOURCE_DIR})
    add_library(metalcpp INTERFACE IMPORTED)
    target_include_directories(metalcpp INTERFACE ${metalcpp_INCLUDE_DIR})
    target_link_libraries(metalcpp INTERFACE
        "-framework Metal"
        "-framework MetalKit"
        "-framework Foundation"
    )

endfunction()
