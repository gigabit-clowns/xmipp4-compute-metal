// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device_queue.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <utility>

namespace xmipp4
{
namespace hardware
{

metal_device_queue::metal_device_queue(metal_device &device)
{
    XMIPP4_ASSERT( device.get_device_handle() );
    m_command_queue = NS::TransferPtr(device.get_device_handle()->newCommandQueue());
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
        XMIPP4_ASSERT( m_command_queue );
        m_command_queue->release();
        m_command_queue = nullptr;
    }
}

NS::SharedPtr<MTL::CommandQueue> metal_device_queue::get_handle() noexcept
{
    return m_command_queue;
}

void metal_device_queue::wait_until_completed() const
{

    XMIPP4_ASSERT( m_command_queue );
    //TODO: el puto oier me ha quitado mis pending buffers
}

bool metal_device_queue::is_idle() const noexcept
{
    //TODO: el puto oier me ha quitado mis pending buffers
    return true;
}

} // namespace hardware
} // namespace xmipp4
