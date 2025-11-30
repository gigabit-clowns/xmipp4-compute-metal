// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device_queue.hpp>

#include <xmipp4/metal/hardware/metal_device.hpp>
#include <xmipp4/metal/hardware/metal_error.hpp>

#include <utility>

namespace xmipp4
{
namespace hardware
{

metal_device_queue::metal_device_queue(metal_device &device)
{
    XMIPP4_ASSERT( device.get_device_handle() );
    m_command_queue = device.get_device_handle()->newCommandQueue();
    XMIPP4_ASSERT( m_command_queue );   
}

metal_device_queue::metal_device_queue(metal_device_queue &&other) noexcept
    : m_command_queue(other.m_command_queue)
{
    other.m_command_queue = nullptr;
}

metal_device_queue::~metal_device_queue()
{
    reset();
}

metal_device_queue&
metal_device_queue::operator=(metal_device_queue &&other) noexcept
{
    swap(other);
    other.reset();
    return *this;
}

void metal_device_queue::swap(metal_device_queue &other) noexcept
{
    std::swap(m_command_queue, other.m_command_queue);
}

void metal_device_queue::reset() noexcept
{
    if (m_command_queue)
    {
        XMIPP4_METAL_CHECK( m_command_queue->release() );
        m_command_queue = nullptr;
    }
}

MTL::CommandQueue *metal_device_queue::get_handle() noexcept
{
    return m_command_queue;
}

void metal_device_queue::wait_until_completed() const
{

    XMIPP4_ASSERT( m_command_queue );
    while (m_pending_buffers.load() > 0)
    {
        std::this_thread::yield(); // sleep 0-1ms
    }
}

bool metal_device_queue::is_idle() const noexcept
{
    return m_pending_buffers.load() == 0;
}

MTL::CommandBuffer* metal_device_queue::create_command_buffer()
{
    XMIPP4_ASSERT(m_command_queue);

    auto cb = m_command_queue->commandBuffer();
    XMIPP4_ASSERT(cb);

    // Increment nr of pending buffers
    m_pending_buffers.fetch_add(1);

    cb->addCompletedHandler([this](MTL::CommandBuffer* buffer){
        m_pending_buffers.fetch_sub(1); // Decrement only after finished
    });

    return cb;
}

} // namespace hardware
} // namespace xmipp4

/**
 * auto cb = m_command_queue->commandBuffer();
 * m_inflight_count.fetch_add(1);
 * cb->addCompletedHandler([this](MTL::CommandBuffer*){
    m_inflight_count.fetch_sub(1);
 * });
 */