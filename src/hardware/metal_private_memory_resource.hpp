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

class metal_device;

class metal_private_memory_resource final
    : public memory_resource
{
public:
    explicit metal_private_memory_resource(metal_device &device) noexcept;
    metal_private_memory_resource(const metal_private_memory_resource &other) = default;
    metal_private_memory_resource(metal_private_memory_resource &&other) = default;
    ~metal_private_memory_resource() override = default;

    metal_private_memory_resource&
    operator=(const metal_private_memory_resource &other) = default;
    metal_private_memory_resource&
    operator=(metal_private_memory_resource &&other) = default;

    metal_device* get_target_device() const noexcept override;

    memory_resource_kind get_kind() const noexcept override;

    std::size_t get_max_heap_alignment() const noexcept override;

    std::shared_ptr<memory_heap>
    create_memory_heap(std::size_t size, std::size_t alignment) override;

private:
    std::reference_wrapper<metal_device> m_device;
};

} // namespace hardware
} // namespace xmipp4
