// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
    void register_at(interface_catalog& catalog) const override;

private:
    static const std::string name;

};

} // namespace xmipp4
