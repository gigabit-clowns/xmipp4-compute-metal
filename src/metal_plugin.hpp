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
 * @file metal_plugin.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @author Mikel Iceta Tena (miceta@cnb.csic.es)
 * @brief Definition of the metal_plugin class
 * @date 2024-11-08
 * 
 */


#include <xmipp4/core/plugin.hpp>

namespace xmipp4 
{

class metal_plugin final
    : public plugin
{
public:
    metal_plugin() = default;
    metal_plugin(const metal_plugin& other) = default;
    metal_plugin(metal_plugin&& other) = default;
    virtual ~metal_plugin() = default;

    metal_plugin& operator=(const metal_plugin& other) = default;
    metal_plugin& operator=(metal_plugin&& other) = default;

    const std::string& get_name() const noexcept override;
    version get_version() const noexcept override;
    void register_at(interface_registry& registry) const override;

private:
    static const std::string name;

};

} // namespace xmipp4
