// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <xmipp4/core/hardware/device_create_parameters.hpp>

#include <memory>
#include <sstream>

namespace xmipp4
{
namespace hardware
{

metal_device::metal_device(NS::SharedPtr<MTL::Device> device, const device_create_parameters &params)
    : m_device(std::move(device))
{
}

// TODO: Come back when this is done:
//      metal_device_to_host_transfer
//      metal_host_to_device_transfer
//      metal_event
//      metal_device_copy
//      metal_host_memory_allocator
//      metal_device_memory_allocator
//      todo lo demas xd

} // namespace hardware
} // namespace xmipp4
