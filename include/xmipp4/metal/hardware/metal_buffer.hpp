// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer.hpp>
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace hardware
{

class metal_buffer final
    : public buffer
{
public:
    metal_buffer(
        NS::SharedPtr<MTL::Buffer> buffer_handle,
        std::size_t size,
        std::reference_wrapper<memory_resource> resource,
        std::unique_ptr<buffer_sentinel> sentinel
    );
    ~metal_buffer() override = default;

private:
   NS::SharedPtr<MTL::Buffer> m_buffer_handle;

};

} // namespace hardware
} // namespace xmipp4