// SPDX-License-Identifier: GPL-3.0-only

#include "metal_memory_heap.hpp"

#include <xmipp4/metal/hardware/metal_device.hpp>
#include <xmipp4/metal/hardware/metal_buffer.hpp>
#include <xmipp4/core/platform/assert.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

namespace xmipp4
{
namespace hardware
{

metal_memory_heap::metal_memory_heap(
    metal_memory_resource &resource,
    MTL::ResourceOptions resource_options,
    std::size_t size
)
    : m_resource(resource)
    , m_heap(nullptr)
{
    const auto *device = resource.get_target_device();
    XMIPP4_ASSERT( device );

    // Generate a descriptor for the heap
    MTL::HeapDescriptor * desc = MTL::HeapDescriptor::alloc()->init();
    desc->setType(MTL::HeapTypePlacement);
    desc->setResourceOptions(resource_options);
    desc->setSize(size);

    // Get device handle
    auto handle = device->get_device_handle();
    XMIPP4_ASSERT( handle );

    // Create the heap
    m_heap = NS::TransferPtr(handle->newHeap(desc));

    // Clean up and check for errors
    desc->release();
    if (!m_heap && size > 0)
    {
        throw std::bad_alloc();
    }
}

std::size_t metal_memory_heap::get_size() const noexcept
{
    return m_heap ? m_heap->size() : 0;
}

std::shared_ptr<buffer> metal_memory_heap::create_buffer(
    std::size_t offset, 
    std::size_t size,
    std::unique_ptr<buffer_sentinel> sentinel
)
{
    if (offset + size > get_size())
    {
        throw std::out_of_range("Allocation exceeds heap bounds");
    }

    if (!m_heap)
    {
        throw invalid_operation_error(
            "Cannot allocate buffer from a null heap"
        );
    }

    NS::SharedPtr<MTL::Buffer> buffer_handle = NS::TransferPtr(
        m_heap->newBuffer(size, m_heap->resourceOptions(), offset)
    );

    return std::make_shared<metal_buffer>(
        std::move(buffer_handle),
        size,
        m_resource,
        std::move(sentinel)
    );
}

} // namespace hardware
} // namespace xmipp4