// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_resource.hpp>

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <utility>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace hardware
{

class metal_memory_resource
    : public memory_resource
{
public:
    explicit metal_memory_resource(metal_device &device) noexcept;
    metal_memory_resource(const metal_memory_resource &other) = default;
    metal_memory_resource(metal_memory_resource &&other) = default;
    ~metal_memory_resource() override = default;

    metal_memory_resource&
    operator=(const metal_memory_resource &other) = default;
    metal_memory_resource&
    operator=(metal_memory_resource &&other) = default;

    metal_device* get_target_device() const noexcept override;

    std::size_t get_max_heap_alignment() const noexcept override;

private:
    std::reference_wrapper<metal_device> m_device;

};

} // namespace xmipp4
} // namespace hardware