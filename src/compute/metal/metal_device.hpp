#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file metal_device.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Defines metal_device class.
 * @date 2024-10-30
 * 
 */

#include <xmipp4/core/compute/device.hpp>

namespace xmipp4 
{
namespace compute
{

class metal_device_backend;

class metal_device final
    : public device
{
public:
    metal_device(int device);
    metal_device(const metal_device &other) = default;
    metal_device(metal_device &&other) = default;
    virtual ~metal_device() = default;

    metal_device& operator=(const metal_device &other) = default;
    metal_device& operator=(metal_device &&other) = default;

    std::unique_ptr<device_queue> create_queue() final;
    std::shared_ptr<device_queue> create_queue_shared() final;

    std::unique_ptr<device_memory_allocator>
    create_device_memory_allocator() final;
    std::shared_ptr<device_memory_allocator>
    create_device_memory_allocator_shared() final;

    std::unique_ptr<host_memory_allocator>
    create_host_memory_allocator() final;
    std::shared_ptr<host_memory_allocator>
    create_host_memory_allocator_shared() final;

    std::unique_ptr<host_to_device_transfer> 
    create_host_to_device_transfer() final;
    std::shared_ptr<host_to_device_transfer> 
    create_host_to_device_transfer_shared() final;

    std::unique_ptr<device_to_host_transfer> 
    create_device_to_host_transfer() final;
    std::shared_ptr<device_to_host_transfer> 
    create_device_to_host_transfer_shared() final;

private:
    int m_device;

}; 

} // namespace compute
} // namespace xmipp4