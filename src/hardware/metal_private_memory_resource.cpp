// SPDX-License-Identifier: GPL-3.0-only

#include "metal_private_memory_resource.hpp"

#include <xmipp4/metal/hardware/metal_error.hpp>
#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/system/host.hpp>

#include "metal_private_memory_heap.hpp"

namespace xmipp4
{
namespace hardware
{

metal_private_memory_resource::metal_private_memory_resource(
    metal_device &device
) noexcept
    : m_device(device)
{
}

metal_device* metal_private_memory_resource::get_target_device() const noexcept
{
    metal_device &device = m_device.get();
    return &device;
}

memory_resource_kind metal_private_memory_resource::get_kind() const noexcept
{
    return memory_resource_kind::device_local;
}

std::size_t metal_private_memory_resource::get_max_heap_alignment() const noexcept
{
    // Should be 4096 in most cases, but we query the system for safety
    // TODO: Oier va a cambiar algo para que sea mas correcto
    return system::get_page_size();
}

std::shared_ptr<memory_heap> metal_private_memory_resource::create_memory_heap(
    std::size_t size,
    std::size_t alignment
)
{
    if (alignment > get_max_heap_alignment())
    {
        throw std::invalid_argument(
            "alignment exceeds the maximum alignment guaranteed by the memory "
            "resource"
        );
    }

    return std::make_shared<metal_private_memory_heap>(*this, size);
}

} // namespace hardware
} // namespace xmipp4