// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_device.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include "metal_private_memory_resource.hpp"
#include "metal_shared_memory_resource.hpp"

#include <memory>
#include <sstream>

namespace xmipp4
{
namespace hardware
{

metal_device::metal_device(NS::SharedPtr<MTL::Device> device)
    : m_device(std::move(device))
    , m_private_memory_resource(std::make_unique<metal_private_memory_resource>(*this))
    , m_shared_memory_resource(std::make_unique<metal_shared_memory_resource>(*this))
{
}

NS::SharedPtr<MTL::Device> metal_device::get_device_handle() const noexcept
{
    return m_device;
}

memory_resource& metal_device::get_device_local_memory_resource()
{
    XMIPP4_ASSERT(m_shared_memory_resource);
    if ( m_shared_memory_resource->get_kind() == memory_resource_kind::unified )
    {
        return *m_shared_memory_resource;
    }
    else
    {
        XMIPP4_ASSERT(m_private_memory_resource);
        return *m_private_memory_resource;
    }
}

memory_resource& metal_device::get_host_accessible_memory_resource()
{
    XMIPP4_ASSERT(m_shared_memory_resource);
    return *m_shared_memory_resource;
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
