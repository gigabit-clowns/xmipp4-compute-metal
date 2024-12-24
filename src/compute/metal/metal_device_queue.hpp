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
 * @file metal_device_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Defines metal_device_queue class.
 * @date 2024-11-12
 * 
 */

#include <xmipp4/core/compute/device_queue.hpp>

#include <Metal/Metal.hpp>

namespace xmipp4 
{
namespace compute
{

class metal_device_queue_backend;

class metal_device_queue final
    : public device_queue
{
public:
    using handle = MTL::CommandQueueDescriptor;

    metal_device_queue(MTL::Device * device);
    metal_device_queue(const metal_device_queue &other) = delete;
    metal_device_queue(metal_device_queue &&other) noexcept;
    virtual ~metal_device_queue();

    metal_device_queue& operator=(const metal_device_queue &other) = delete;
    metal_device_queue& operator=(metal_device_queue &&other) noexcept;

    void swap(metal_device_queue &other) noexcept;
    void reset() noexcept;
    handle get_handle() noexcept;

    void synchronize() const final;

private:
    handle m_stream;

}; 

} // namespace compute
} // namespace xmipp4
