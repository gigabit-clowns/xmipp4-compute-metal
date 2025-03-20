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
 * @file metal_plugin.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Implementation of metal_plugin.hpp
 * @date 2024-11-08
 * 
 */

#include "metal_plugin.hpp"

#include "compute/metal/metal_device_backend.hpp"

#include <xmipp4/core/interface_registry.hpp>
#include <xmipp4/core/compute/device_manager.hpp>

namespace xmipp4 
{

const std::string metal_plugin::name = "xmipp4-compute-metal";

const std::string& metal_plugin::get_name() const noexcept
{
    return name; 
}

version metal_plugin::get_version() const noexcept
{
    return version(
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_PATCH
    );
}

void metal_plugin::register_at(interface_registry& registry) const
{
    compute::metal_device_backend::register_at(
        registry.get_interface_manager<compute::device_manager>()
    );
}

} // namespace xmipp4
