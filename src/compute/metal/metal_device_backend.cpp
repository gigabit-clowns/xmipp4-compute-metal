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
 * @file metal_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Implementation of metal_device_backend.hpp
 * @date 2024-11-08
 * 
 */

#include "metal_device_backend.hpp"

#include "metal_device.hpp"

#include <xmipp4/core/compute/device_manager.hpp>

#include <numeric>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace xmipp4
{
namespace compute
{

const std::string metal_device_backend::m_name = "metal";

const std::string& metal_device_backend::get_name() const noexcept
{
    return m_name;
}

version metal_device_backend::get_version() const noexcept
{
    // Metal does not have a notion of "version"
    return version(0,0,0);
}

bool metal_device_backend::is_available() const noexcept
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int count = sizeof(*devs);
    return count > 0;
}

void metal_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int count = sizeof(devs);
    
    ids.resize(count);
    std::iota(
        ids.begin(), ids.end(),
        static_cast<std::size_t>(0)
    );
}

bool metal_device_backend::get_device_properties(std::size_t id, 
                                                device_properties &desc ) const
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int count = sizeof(&devs);
    const auto device = static_cast<int>(id);
    const auto result = device < count;

    if (result)
    {
        MTL::Device * dev = static_cast<MTL::Device *>(devs->object(0));

        device_type type;
        std::string location;
        switch (dev->location()) {
            case MTL::DeviceLocationBuiltIn:
                type = device_type::integrated_gpu;
                location = "Apple Silicon, Intel Iris or Discrete laptop GPU";
                break;
            case MTL::DeviceLocationSlot:
                type = device_type::gpu;
                location = std::to_string(dev->locationNumber());
                break;
            case MTL::DeviceLocationExternal:
                type = device_type::gpu;
                location = "External Enclosure";
                break;
        }
        // Write
        desc.set_name(dev->name());
        desc.set_physical_location(std::move(location));
        desc.set_type(type);
        desc.set_total_memory_bytes(dev->recommendedMaxWorkingSetSize());
    }

    return result;
}

std::unique_ptr<device> 
metal_device_backend::create_device(std::size_t id)
{
    const NS::Array * devs = MTL::CopyAllDevices();
    int count = sizeof(devs);
    if (static_cast<int>(id) >= count)
    {
        throw std::invalid_argument("Invalid device id");
    }

    return std::make_unique<metal_device>(id);
}

std::shared_ptr<device> 
metal_device_backend::create_device_shared(std::size_t id)
{
    const NS::Array * devs = MTL::CopyAllDevices();
    int count = sizeof(devs);
    if (static_cast<int>(id) >= count)
    {
        throw std::invalid_argument("Invalid device id");
    }

    return std::make_shared<metal_device>(id);
}

bool metal_device_backend::register_at(device_manager &manager)
{
    return manager.register_backend(std::make_unique<metal_device_backend>());
}

} // namespace compute
} // namespace xmipp4
