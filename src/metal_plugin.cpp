// SPDX-License-Identifier: GPL-3.0-only

#include "metal_plugin.hpp"

#include "compute/metal/metal_device_backend.hpp"

#include <xmipp4/core/interface_registry.hpp>
#include <xmipp4/core/compute/device_manager.hpp>

namespace xmipp4 
{

const std::string metal_plugin::name = "xmipp4-compute-metal";

const std::string& metal_plugin::get_name() const noexcept
{
    return name; 
}

version metal_plugin::get_version() const noexcept
{
    return version(
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_PATCH
    );
}

void metal_plugin::register_at(interface_registry& registry) const
{
    compute::metal_device_backend::register_at(
        registry.get_interface_manager<compute::device_manager>()
    );
}

} // namespace xmipp4
