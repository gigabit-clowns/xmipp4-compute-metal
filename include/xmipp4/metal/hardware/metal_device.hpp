// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace hardware
{

class device_create_parameters;

class metal_device final
    : public device
{
  public:
    metal_device(NS::SharedPtr<MTL::Device> device);
    metal_device(const metal_device &other) = delete;
    metal_device(metal_device &&other) = default;
    ~metal_device() override = default;

    metal_device& operator=(const metal_device &other) = delete;
    metal_device& operator=(metal_device &&other) = default;

    NS::SharedPtr<MTL::Device> get_device_handle() const noexcept;

    void enumerate_memory_resources(
        std::vector<memory_resource*> &resources
    ) override;

    bool can_access_memory_resource(
      const memory_resource &resource
    ) const override;

    std::shared_ptr<device_queue>
    create_device_queue() override;
  
    std::shared_ptr<device_event> create_device_event() override;

    std::shared_ptr<device_to_host_event>
    create_device_to_host_event() override;

  private:
      NS::SharedPtr<MTL::Device> m_device;

};

} // namespace hardware
} // namespace xmipp4
