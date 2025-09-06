// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <Metal/Metal.hpp>
#include <xmipp4/core/compute/device.hpp>

#include "metal_device_queue_pool.hpp"

namespace xmipp4
{
namespace compute
{

class device_create_parameters;

class metal_device final
    : public device
{
  public:
    metal_device(int device, const device_create_parameters &params);
    metal_device(const metal_device &other) = delete;
    metal_device(metal_device &&other) = default;
    ~metal_device() override = default;

    metal_device& operator=(const metal_device &other) = delete;
    metal_device& operator=(metal_device &&other) = default;

    int get_index() const noexcept;
    MTL::Device * get_device_handle() const noexcept;

    metal_device_queue_pool& get_queue_pool() override;

    std::shared_ptr<device_memory_allocator>
    create_device_memory_allocator() override;

    std::shared_ptr<host_memory_allocator>
    create_host_memory_allocator() override;

    std::shared_ptr<host_to_device_transfer>
    create_host_to_device_transfer() override;

    std::shared_ptr<device_to_host_transfer>
    create_device_to_host_transfer() override;

    std::shared_ptr<device_copy>
    create_device_copy() override;

    std::shared_ptr<device_event>
    create_device_event() override;

    std::shared_ptr<device_to_host_event>
    create_device_to_host_event() override;

    private:
        MTL::Device * m_device;
        int m_index;
        metal_device_queue_pool m_queue_pool;

};

} // namespace compute
} // namespace xmipp4