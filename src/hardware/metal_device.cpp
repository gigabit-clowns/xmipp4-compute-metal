// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <xmipp4/core/hardware/device.hpp>

#include <memory>
#include <sstream>

namespace xmipp4
{
namespace hardware
{

metal_device::metal_device(NS::SharedPtr<MTL::Device> device)
    : m_device(std::move(device))
{
}

NS::SharedPtr<MTL::Device> metal_device::get_device_handle() const noexcept
{
    return m_device;
}

void metal_device::enumerate_memory_resources(
    std::vector<memory_resource*> &resources
)
{
    return; // TODO
}

bool metal_device::can_access_memory_resource(
  const memory_resource &resource
) const
{
    return false; // TODO
}

std::shared_ptr<device_queue>
metal_device::create_device_queue()
{
    return nullptr; // TODO
    //return std::make_shared<metal_device_queue>(*this);
}

std::shared_ptr<device_event> metal_device::create_device_event()
{
    return nullptr; // TODO
    //return std::make_shared<metal_event>();
}

std::shared_ptr<device_to_host_event>
metal_device::create_device_to_host_event()
{
    return nullptr; // TODO
    //return std::make_shared<metal_event>();
} 


// TODO: Come back when this is done:
//      metal_event
//      metal_device_queue
//      todo lo demas xd

} // namespace hardware
} // namespace xmipp4
