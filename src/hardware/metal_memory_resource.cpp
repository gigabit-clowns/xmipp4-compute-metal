// SPDX-License-Identifier: GPL-3.0-only

#include "metal_memory_resource.hpp"

#include <xmipp4/metal/hardware/metal_error.hpp>
#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/system/host.hpp>

namespace xmipp4
{
namespace hardware
{
metal_memory_resource::metal_memory_resource(
    metal_device &device
) noexcept
    : m_device(device)
{
}

metal_device* metal_memory_resource::get_target_device() const noexcept
{
    metal_device &device = m_device.get();
    return &device;
}

std::size_t metal_memory_resource::get_max_heap_alignment() const noexcept
{
    // Should be 4096 in most cases, but we query the system for safety
    // TODO: Oier va a cambiar algo para que sea mas correcto
    return system::get_page_size();
}

} // namespace hardware
} // namespace xmipp4