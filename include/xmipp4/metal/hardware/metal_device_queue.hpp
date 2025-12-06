// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device_queue.hpp>

#include "../dynamic_shared_object.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <iostream>

namespace xmipp4 
{
namespace hardware
{

class metal_device;

class metal_device_queue final
    : public device_queue
{
    public:
        using handle = MTL::CommandQueue*;

        XMIPP4_HARDWARE_METAL_API
        explicit metal_device_queue(metal_device &device);
        metal_device_queue(const metal_device_queue &other) = delete;
        XMIPP4_HARDWARE_METAL_API
        metal_device_queue(metal_device_queue &&other) noexcept;
        XMIPP4_HARDWARE_METAL_API
        ~metal_device_queue() override;

        metal_device_queue& operator=(const metal_device_queue &other) = delete;
        XMIPP4_HARDWARE_METAL_API
        metal_device_queue& operator=(metal_device_queue &&other) noexcept;

        XMIPP4_HARDWARE_METAL_API
        void swap(metal_device_queue &other) noexcept;

        XMIPP4_HARDWARE_METAL_API
        void reset() noexcept;

        XMIPP4_HARDWARE_METAL_API
        NS::SharedPtr<MTL::CommandQueue> get_handle() noexcept;

        XMIPP4_HARDWARE_METAL_API
        void wait_until_completed() const override;

        XMIPP4_HARDWARE_METAL_API
        bool is_idle() const noexcept override;

    private:
        NS::SharedPtr<MTL::CommandQueue> m_command_queue;

};

} // namespace hardware
} // namespace xmipp4