// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_heap.hpp>

#include <utility>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace hardware
{

class metal_private_memory_resource;

class metal_private_memory_heap final
    : public memory_heap
{
public:
    metal_private_memory_heap(
        metal_private_memory_resource &resource,
        std::size_t size
    );
    ~metal_private_memory_heap() override;

    std::size_t get_size() const noexcept override;

    std::shared_ptr<buffer> create_buffer(
        std::size_t offset, 
        std::size_t size,
        std::unique_ptr<buffer_sentinel> sentinel
    ) override;

private:
    std::reference_wrapper<metal_private_memory_resource> m_resource;
    NS::SharedPtr<MTL::Heap> m_heap;
    std::size_t m_size;
    
};

} // namespace hardware
} // namespace xmipp4
