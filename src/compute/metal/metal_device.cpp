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
 * @file metal_device.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Implementation of metal_device.hpp
 * @date 2024-10-30
 * 
 */

#include "metal_device.hpp"

#include "metal_device_queue.hpp"
#include "metal_device_memory_allocator.hpp"
#include "metal_host_memory_allocator.hpp"
#include "metal_device_to_host_transfer.hpp"
#include "metal_host_to_device_transfer.hpp"

#include <memory>

namespace xmipp4
{
namespace compute
{

metal_device::metal_device(int device)
    : m_device(device)
{
}

std::unique_ptr<device_queue> metal_device::create_queue()
{
    return std::make_unique<metal_device_queue>(m_device);
}

std::shared_ptr<device_queue> metal_device::create_queue_shared()
{
    return std::make_shared<metal_device_queue>(m_device);
}

std::unique_ptr<device_memory_allocator> 
metal_device::create_device_memory_allocator()
{
    return std::make_unique<metal_device_memory_allocator>();
}

std::shared_ptr<device_memory_allocator> 
metal_device::create_device_memory_allocator_shared()
{
    return std::make_shared<metal_device_memory_allocator>();
}

std::unique_ptr<host_memory_allocator> 
metal_device::create_host_memory_allocator()
{
    return std::make_unique<metal_host_memory_allocator>();
}

std::shared_ptr<host_memory_allocator> 
metal_device::create_host_memory_allocator_shared()
{
    return std::make_shared<metal_host_memory_allocator>();
}

std::unique_ptr<host_to_device_transfer> 
metal_device::create_host_to_device_transfer()
{
    return std::make_unique<metal_host_to_device_transfer>();
}

std::shared_ptr<host_to_device_transfer> 
metal_device::create_host_to_device_transfer_shared()
{
    return std::make_shared<metal_host_to_device_transfer>();
}

std::unique_ptr<device_to_host_transfer> 
metal_device::create_device_to_host_transfer()
{
    return std::make_unique<metal_device_to_host_transfer>();
}

std::shared_ptr<device_to_host_transfer> 
metal_device::create_device_to_host_transfer_shared()
{
    return std::make_shared<metal_device_to_host_transfer>();
}

} // namespace compute
} // namespace xmipp4
