// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device_backend.hpp>

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <xmipp4/core/hardware/device_manager.hpp>

#include <cstdlib>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <sys/utsname.h>

namespace xmipp4
{
namespace hardware
{

std::string metal_device_backend::get_name() const noexcept
{
	return "metal";
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
	NS::SharedPtr<NS::Array> devs = NS::TransferPtr(MTL::CopyAllDevices());
	const int count = devs->count();
	return (count > 0);
}

backend_priority metal_device_backend::get_priority() const noexcept
{
	return backend_priority::normal;
}

void metal_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
	NS::SharedPtr<NS::Array> devs = NS::TransferPtr(MTL::CopyAllDevices());
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
	NS::SharedPtr<MTL::Device> dev = get_metal_device_handle(id);
	if (dev)
	{
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
		// Write values
		desc.set_name(dev->name()->cString(NS::StringEncoding::UTF8StringEncoding));
		desc.set_physical_location(std::move(location));
		desc.set_type(type);
		desc.set_total_memory_bytes(dev->recommendedMaxWorkingSetSize());
		return true;
	}
	return false;
}

std::shared_ptr<device> 
metal_device_backend::create_device(std::size_t id)
{
	NS::SharedPtr<MTL::Device> dev = get_metal_device_handle(id);

	if (!dev){
		throw std::invalid_argument("Requested device ID is invalid.");
	}
	
	return std::make_shared<metal_device>(dev);
}

bool metal_device_backend::register_at(device_manager &manager)
{
	return manager.register_backend(std::make_unique<metal_device_backend>());
}

NS::SharedPtr<MTL::Device> 
metal_device_backend::get_metal_device_handle(std::size_t id)
{
	NS::SharedPtr<NS::Array> devs = NS::TransferPtr(MTL::CopyAllDevices());
	const int index = static_cast<int>(id);
	const int count = static_cast<int>(devs->count());
	if (index < count)
	{ // OK
		return NS::TransferPtr(static_cast<MTL::Device *>(devs->object(index)));
	}
	else
	{ // Amiga, la cagaste
		return nullptr;
	}
}

} // namespace hardware
} // namespace xmipp4
