// SPDX-License-Identifier: GPL-3.0-only

#include "device_backend.hpp"

#include "device.hpp"

#include <xmipp4/core/hardware/device_manager.hpp>

#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>

#include <sys/utsname.h>

namespace xmipp4
{
namespace metal
{

std::string device_backend::get_name() const
{
	return "metal";
}

version device_backend::get_version() const
{
	// Metal does not have a notion of "version". Report the running Darwin
	// version instead.
	struct utsname sys_info;
	if (uname(&sys_info) == 0)
	{
		std::string release = sys_info.release;
		std::istringstream iss(release);
		int major;
		int minor;
		int patch;
		char dot;

		iss >> major >> dot >> minor >> dot >> patch;
		return version(major, minor, patch);
	}
	return version(0, 0, 0);
}

void device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
	const auto devs = NS::TransferPtr(MTL::CopyAllDevices());
	const auto count = devs->count();

	// In Metal, devices do not have an index of their own; CopyAllDevices'
	// order is taken as invariant for the lifetime of the process.
	ids.resize(static_cast<std::size_t>(count));
	std::iota(ids.begin(), ids.end(), static_cast<std::size_t>(0));
}

bool device_backend::get_device_properties(
	std::size_t id,
	device_properties &desc
) const
{
	const auto dev = get_metal_device_handle(id);
	if (!dev)
	{
		return false;
	}

	device_type type;
	std::string location;
	switch (dev->location())
	{
	case MTL::DeviceLocationBuiltIn:
		type = device_type::integrated_gpu;
		location = "SoC Built-in GPU";
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

	desc.set_name(dev->name()->cString(NS::StringEncoding::UTF8StringEncoding));
	desc.set_physical_location(std::move(location));
	desc.set_type(type);
	desc.set_total_memory_bytes(dev->recommendedMaxWorkingSetSize());
	return true;
}

std::shared_ptr<xmipp4::device>
device_backend::create_device(std::size_t id) const
{
	auto dev = get_metal_device_handle(id);
	if (!dev)
	{
		throw std::invalid_argument("Requested device ID is invalid.");
	}

	return std::make_shared<device>(std::move(dev));
}

bool device_backend::register_at(xmipp4::device_manager &manager)
{
	return manager.register_backend(std::make_unique<device_backend>());
}

NS::SharedPtr<MTL::Device>
device_backend::get_metal_device_handle(std::size_t id)
{
	const auto devs = NS::TransferPtr(MTL::CopyAllDevices());
	const auto index = static_cast<int>(id);
	const auto count = static_cast<int>(devs->count());

	if (index >= count)
	{
		return nullptr;
	}

	// object() returns an autoreleased reference, unlike CopyAllDevices()
	// itself, so it must be retained rather than transferred.
	return NS::RetainPtr(static_cast<MTL::Device*>(devs->object(index)));
}

} // namespace metal
} // namespace xmipp4
