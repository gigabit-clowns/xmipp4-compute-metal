// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/hardware/metal_buffer.hpp>

namespace xmipp4
{
namespace hardware
{
metal_buffer::metal_buffer(
    NS::SharedPtr<MTL::Buffer> buffer_handle,
    std::size_t size,
    std::reference_wrapper<memory_resource> resource,
    std::unique_ptr<buffer_sentinel> sentinel
)
    : buffer(buffer_handle->contents(), size, resource, std::move(sentinel))
    , m_buffer_handle(std::move(buffer_handle))
{
}

} // namespace hardware
} // namespace xmipp4