// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/device_queue_pool.hpp>
#include "metal_device_queue.hpp"

#include <vector>

namespace xmipp4 
{
namespace compute
{
/**
 * @brief Implementation of the device_queue_pool interface to be 
 * able to obtain metal_device_queue-s.
 */
class metal_device_queue_pool final
    : public device_queue_pool
{
public:
    metal_device_queue_pool(int device_index, std::size_t count);
    metal_device_queue_pool(const metal_device_queue_pool &other) = delete;
    metal_device_queue_pool(metal_device_queue_pool &&other) = default;
    ~metal_device_queue_pool() override = default;

    metal_device_queue_pool&
    operator=(const metal_device_queue_pool &other) = delete;
    metal_device_queue_pool&
    operator=(metal_device_queue_pool &&other) = default;

    std::size_t get_size() const noexcept override;
    metal_device_queue& get_queue(std::size_t index) override;

    private:
        std::vector<metal_device_queue> m_queues;
        
};

} // namespace compute
} // namespace xmipp4