// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/compute/metal_device_queue.hpp>

#include <xmipp4/metal/compute/metal_device.hpp>
#include <xmipp4/metal/compute/metal_error.hpp>

#include <utility>

namespace xmipp4
{
namespace compute
{

metal_device_queue::metal_device_queue()
{
    MTL::Device * dev = XMIPP4_METAL_CHECK(MTL::CreateSystemDefaultDevice());
    m_stream = XMIPP4_METAL_CHECK(dev->newCommandQueue());
    m_last_cmd = nullptr;
}

metal_device_queue::metal_device_queue(metal_device &device)
{
    MTL::Device * dev = XMIPP4_METAL_CHECK(device.get_device_handle());
    m_stream = XMIPP4_METAL_CHECK(dev->newCommandQueue());
    m_last_cmd = nullptr;
}

metal_device_queue::metal_device_queue(metal_device_queue &&other) noexcept
    : m_stream(other.m_stream),
      m_last_cmd(other.m_last_cmd)
{
    other.m_stream = nullptr;
    other.m_last_cmd = nullptr;
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
    std::swap(m_stream, other.m_stream);
    std::swap(m_last_cmd, other.m_last_cmd);
}

void metal_device_queue::reset() noexcept
{
    if (m_last_cmd){
        m_last_cmd->waitUntilCompleted(); // Safeguard
        m_last_cmd->release();
    }
    if (m_stream)
    {
        m_stream->release();
    }

}

metal_device_queue::handle metal_device_queue::get_handle() noexcept
{
    return m_stream;
}

void metal_device_queue::wait_until_completed() const
{
    // Create a command buffer, commit it, and wait until completed
    // Metal buffers are single-use, contained by queues
    // Only way to emulate a CUDA "wait all" is...
    // Add a fence command to the end and wait for it!
    if (m_stream && m_last_cmd)
    {
        m_last_cmd->waitUntilCompleted();      
    }
}

bool metal_device_queue::is_idle() const noexcept
{
    // Basic cases: no stream or stream but no last command buffer
    if (!m_stream) {
        return true; // No stream, no work -> Idle
    }
    if (!m_last_cmd) {
        return true; // No last command -> Idle
    }
    
    auto status = m_last_cmd->status();
    return (status == MTL::CommandBufferStatus::CommandBufferStatusCompleted ||
            status == MTL::CommandBufferStatus::CommandBufferStatusError);
}

} // namespace compute
} // namespace xmipp4