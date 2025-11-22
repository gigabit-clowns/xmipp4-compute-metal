// SPDX-License-Identifier: GPL-3.0-only

#include "metal_private_memory_heap.hpp"

#include <xmipp4/metal/hardware/metal_error.hpp>
#include <xmipp4/metal/hardware/metal_device.hpp>
#include <xmipp4/metal/hardware/metal_buffer.hpp>
#include <xmipp4/core/platform/assert.hpp>
#include <xmipp4/core/platform/constexpr.hpp>
#include <xmipp4/core/memory/align.hpp>

#include "metal_private_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

metal_private_memory_heap::metal_private_memory_heap(
    metal_private_memory_resource &resource,
    std::size_t size
)
    : m_resource(resource)
    , m_heap(nullptr)
    , m_size(size)
{
    const auto *device = resource.get_target_device();
    XMIPP4_ASSERT( device );

    // Generate a dezcriptor for the heap
    MTL::HeapDescriptor * desc = MTL::HeapDescriptor::alloc();
    desc->setSize(size);
    desc->setStorageMode(MTL::StorageMode::StorageModePrivate);

    // Instantiate a heap from the device
    m_heap = NS::TransferPtr(device->get_device_handle()->newHeap(desc));

    // Clean up and check for errors
    desc->release();
    if (!m_heap && size > 0)
    {
        throw std::bad_alloc();
    }

}

metal_private_memory_heap::~metal_private_memory_heap()
{
    if (m_heap)
    {
        m_heap->release();
    }
}

std::size_t metal_private_memory_heap::get_size() const noexcept
{
    return m_size;
}

std::shared_ptr<buffer> metal_private_memory_heap::create_buffer(
    std::size_t offset, 
    std::size_t size,
    std::unique_ptr<buffer_sentinel> sentinel
)
{
    if (offset + size >= m_size)
    {
        throw std::out_of_range("Allocation exceeds heap bounds");
    }

    NS::SharedPtr<MTL::Buffer> buffer_handle = NS::TransferPtr(m_heap->newBuffer(m_size, MTL::ResourceStorageModePrivate));

    return std::make_shared<metal_buffer>(
        std::move(buffer_handle),
        size,
        m_resource,
        std::move(sentinel)
    );
}

} // namespace hardware
} // namespace xmipp4