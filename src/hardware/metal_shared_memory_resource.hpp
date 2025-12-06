// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "metal_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

class metal_shared_memory_resource final
    : public metal_memory_resource
{
public:
    explicit metal_shared_memory_resource(metal_device &device) noexcept;
    ~metal_shared_memory_resource() override = default;

    memory_resource_kind get_kind() const noexcept override;

    std::shared_ptr<memory_heap> create_memory_heap(
        std::size_t size, 
        std::size_t alignment
    ) override;
    
};

} // namespace xmipp4
} // namespace hardware
