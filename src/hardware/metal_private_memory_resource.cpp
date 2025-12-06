// SPDX-License-Identifier: GPL-3.0-only

#include "metal_private_memory_resource.hpp"
#include "metal_memory_heap.hpp"

namespace xmipp4
{
namespace hardware
{

metal_private_memory_resource::metal_private_memory_resource(
    metal_device &device
) noexcept
    : metal_memory_resource(device)
{
}

memory_resource_kind metal_private_memory_resource::get_kind() const noexcept
{
    return memory_resource_kind::device_local;
}

std::shared_ptr<memory_heap> metal_private_memory_resource::create_memory_heap(
    std::size_t size, 
    std::size_t alignment
)
{
    // Check alignment
    if (alignment > get_max_heap_alignment()) {
        throw std::invalid_argument(
            "Requested alignment exceeds maximum allowed alignment"
        );
    }

    return std::make_shared<metal_memory_heap>(
        *this,
        MTL::ResourceStorageModePrivate,
        size
    );
}

} // namespace hardware
} // namespace xmipp4
