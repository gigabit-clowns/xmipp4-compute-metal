// SPDX-License-Identifier: GPL-3.0-only

#include "metal_plugin.hpp"

#include "hardware/metal_device_backend.hpp"

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>

namespace xmipp4 
{

const std::string metal_plugin::name = "xmipp4-hardware-metal";

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

void metal_plugin::register_at(service_catalog& catalog) const
{
    hardware::metal_device_backend::register_at(
        catalog.get_service_manager<hardware::device_manager>()
    );
}

} // namespace xmipp4
