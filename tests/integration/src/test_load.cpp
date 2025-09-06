// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/plugin_manager.hpp>
#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/platform/operating_system.h>

using namespace xmipp4;


static std::string get_metal_plugin_path()
{
    #if XMIPP4_WINDOWS || XMIPP4_LINUX
        #error "Windows and Linux are not supported in compute-metal.";
    #elif XMIPP4_APPLE
        return "./libxmipp4-compute-metal.dylib";
    #else
        #error "Unknown platform"
    #endif
}

TEST_CASE( "load and register xmipp4-compute-metal plugin", "[compute-metal]" ) 
{
    plugin_manager manager;

    const auto* metal_plugin = 
        manager.load_plugin(get_metal_plugin_path());

    REQUIRE( metal_plugin != nullptr );
    REQUIRE( metal_plugin->get_name() == "xmipp4-compute-metal" );
}
