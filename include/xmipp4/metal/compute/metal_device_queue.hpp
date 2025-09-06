// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/device_queue.hpp>

#include <Metal/Metal.hpp>

namespace xmipp4 
{
namespace compute
{
class metal_device;

class metal_device_queue final
    : public device_queue
{
public:
    using handle = MTL::CommandQueue*;
    using last = MTL::CommandBuffer*;

    metal_device_queue();
    explicit metal_device_queue(metal_device &device);
    metal_device_queue(const metal_device_queue &other) = delete;
    metal_device_queue(metal_device_queue &&other) = default;
    ~metal_device_queue() override;

    metal_device_queue& operator=(const metal_device_queue &other) = delete;
    metal_device_queue& operator=(metal_device_queue &&other) = default;

    void swap(metal_device_queue &other) noexcept;
    void reset() noexcept;
    handle get_handle() noexcept;

    void wait_until_completed() const override;
    bool is_idle() const noexcept override;

    private:
        handle m_stream;
        mutable last m_last_cmd;


};

} // namespace compute
} // namespace xmipp4