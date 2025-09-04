// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/device_backend.hpp>

namespace xmipp4 
{
namespace compute
{

class device_manager;



class metal_device_backend final
    : public device_backend
{
public:
    std::string get_name() const noexcept final;
    version get_version() const noexcept final;
    bool is_available() const noexcept final;
    backend_priority get_priority() const noexcept final;

    void enumerate_devices(std::vector<std::size_t> &ids) const final;
    bool get_device_properties(std::size_t id, device_properties &desc) const final;

    std::unique_ptr<device> create_device(std::size_t id, const device_create_parameters &params) final;
    std::shared_ptr<device> create_device_shared(std::size_t id, const device_create_parameters &params) final;

    static bool register_at(device_manager &manager);

}; 

} // namespace compute
} // namespace xmipp4
