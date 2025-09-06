// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/compute/metal_device_queue_pool.hpp>

#include <xmipp4/metal/compute/metal_error.hpp>
#include <xmipp4/metal/compute/metal_device.hpp>
#include <xmipp4/metal/compute/metal_device_backend.hpp>

namespace xmipp4
{
namespace compute
{   

metal_device_queue_pool::metal_device_queue_pool(int device_index, std::size_t count)
{
    XMIPP4_METAL_CHECK(metal_device_backend::get_metal_device_handle(device_index));
    m_queues.resize(count);
}

std::size_t metal_device_queue_pool::get_size() const noexcept
{
    return m_queues.size();
}

metal_device_queue& metal_device_queue_pool::get_queue(std::size_t index)
{
    return m_queues.at(index);
}

} // namespace compute
} // namespace xmipp4