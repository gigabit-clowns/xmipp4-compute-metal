// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_heap.hpp>

#include "metal_memory_resource.hpp"

#include <utility>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace hardware
{

class metal_memory_heap final
    : public memory_heap
{
public:
    metal_memory_heap(
        metal_memory_resource &resource,
        MTL::ResourceOptions resource_options,
        std::size_t size
    );
    ~metal_memory_heap() override = default;

    std::size_t get_size() const noexcept override;

    std::shared_ptr<buffer> create_buffer(
        std::size_t offset, 
        std::size_t size,
        std::unique_ptr<buffer_sentinel> sentinel
    ) override;

private:
    std::reference_wrapper<metal_memory_resource> m_resource;
    NS::SharedPtr<MTL::Heap> m_heap;
    
};

} // namespace hardware
} // namespace xmipp4
