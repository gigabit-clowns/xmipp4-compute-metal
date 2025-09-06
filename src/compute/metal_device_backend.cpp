// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/compute/metal_device_backend.hpp>

#include <xmipp4/metal/compute/metal_error.hpp>
#include <xmipp4/metal/compute/metal_device.hpp>

#include <xmipp4/core/compute/device_manager.hpp>

#include <numeric>
#include <sys/utsname.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>

#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace compute
{

std::string metal_device_backend::get_name() const noexcept
{
    return "Apple Metal on Darwin";
}

version metal_device_backend::get_version() const noexcept
{
    // Metal does not have a notion of "version"
    // Thus, we return the version of the running Darwin
    struct utsname sys_info;
    if (uname(&sys_info) == 0)
    {
        std::string release = sys_info.release;
        std::istringstream iss(release);
        int major, minor, patch;
        char dot;
        iss >> major >> dot >> minor >> dot >> patch;
        return version(major, minor, patch);
    }
    return version(0, 0, 0);
}

bool metal_device_backend::is_available() const noexcept
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int count = devs->count();
    return (count > 0);
}

backend_priority metal_device_backend::get_priority() const noexcept
{
    return backend_priority::normal;
}

void metal_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int count = devs->count();
    
    ids.clear();
    ids.resize(static_cast<std::size_t>(count));
    // In Metal, devices do not have an index
    // We take CopyAllDevices order as invariant
    // Apple Silicon -> Only [0] will exist!    
    std::iota(
        ids.begin(), ids.end(),
        static_cast<std::size_t>(0)
    );
}

bool metal_device_backend::get_device_properties(std::size_t id, 
                                                device_properties &desc ) const
{
    MTL::Device * dev = XMIPP4_METAL_CHECK(get_metal_device_handle(id));
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
        default:
            type = device_type::unknown;
            location = "Unspecified location";
            break;
    }

    // Write
    desc.set_name(dev->name()->cString(NS::StringEncoding::UTF8StringEncoding));
    desc.set_physical_location(std::move(location));
    desc.set_type(type);
    desc.set_total_memory_bytes(dev->recommendedMaxWorkingSetSize());

    return (dev != nullptr);
}

std::shared_ptr<device> 
metal_device_backend::create_device(std::size_t id, 
                                           const device_create_parameters &params)
{
    MTL::Device * dev = XMIPP4_METAL_CHECK(get_metal_device_handle(id));

    return std::make_shared<metal_device>(id, params);
}

bool metal_device_backend::register_at(device_manager &manager)
{
    return manager.register_backend(std::make_unique<metal_device_backend>());
}

static MTL::Device * get_metal_device_handle(std::size_t id)
{
    NS::Array * devs = MTL::CopyAllDevices();
    const int dev = static_cast<int>(id);
    const int count = static_cast<int>(devs->count());
    if (dev < count)
    { // OK
        return static_cast<MTL::Device *>(devs->object(dev));
    }
    else
    { // Amiga, la cagaste
        std::ostringstream oss;
        oss << "METAL error: device " << dev << "is out of bounds.\n" 
        << std::endl;
        throw metal_error(oss.str());
    }
}

} // namespace compute
} // namespace xmipp4
