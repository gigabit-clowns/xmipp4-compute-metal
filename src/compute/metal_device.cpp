// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/compute/metal_device.hpp>

#include <xmipp4/metal/compute/metal_device_queue.hpp>
#include <xmipp4/metal/compute/metal_error.hpp>
#include <xmipp4/metal/compute/metal_device_backend.hpp>
//#include <xmipp4/metal/compute/cuda_device_memory_allocator.hpp>
//#include <xmipp4/metal/compute/cuda_host_memory_allocator.hpp>
//#include <xmipp4/metal/compute/cuda_device_to_host_transfer.hpp>
//#include <xmipp4/metal/compute/cuda_host_to_device_transfer.hpp>
//#include <xmipp4/metal/compute/cuda_device_copy.hpp>
//#include <xmipp4/metal/compute/cuda_event.hpp>

#include <xmipp4/core/compute/device_create_parameters.hpp>

#include <memory>
#include <sstream>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace compute
{

metal_device::metal_device(int device, const device_create_parameters &params)
    : m_device(metal_device_backend::get_metal_device_handle(device))
    , m_index(device)
    , m_queue_pool(device, params.get_desired_queue_count())
{
}

int metal_device::get_index() const noexcept
{
    return 0;
}

metal_device_queue_pool& metal_device::get_queue_pool()
{
    return m_queue_pool;
}

// TODO: Come back when this is done:
//      metal_device_to_host_transfer
//      metal_host_to_device_transfer
//      metal_event
//      metal_device_copy
//      metal_host_memory_allocator
//      metal_device_memory_allocator

} // namespace compute
} // namespace xmipp4