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
 * @file metal_device_queue.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Implementation of metal_device_queue.hpp
 * @date 2024-11-12
 * 
 */

#include "metal_device_queue.hpp"

#include <utility>

namespace xmipp4
{
namespace compute
{

metal_device_queue::metal_device_queue(MTL::Device * device)
{
    //metalSetDevice(device);
    //metalStreamCreate(&m_stream); // TODO check
    device->newCommandQueue(&m_stream);
}

metal_device_queue::metal_device_queue(metal_device_queue &&other) noexcept
    : m_stream(other.m_stream)
{
    other.m_stream = nullptr;
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
}

void metal_device_queue::reset() noexcept
{
    if (m_stream)
    {
        metalStreamDestroy(m_stream);// TODO check
    }
}


metal_device_queue::handle metal_device_queue::get_handle() noexcept
{
    return m_stream;
}

void metal_device_queue::synchronize() const
{
    metalStreamSynchronize(m_stream);
}

} // namespace compute
} // namespace xmipp4
