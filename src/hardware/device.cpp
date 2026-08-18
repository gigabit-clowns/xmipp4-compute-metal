// SPDX-License-Identifier: GPL-3.0-only

#include "device.hpp"

#include "command_queue.hpp"
#include "event.hpp"

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include <utility>

namespace xmipp4
{
namespace metal
{

device::device(NS::SharedPtr<MTL::Device> handle) noexcept
	: m_handle(std::move(handle))
{
}

device::~device() = default;

const NS::SharedPtr<MTL::Device>& device::get_handle() const noexcept
{
	return m_handle;
}

const memory_resource&
device::get_memory_resource(memory_resource_affinity /*affinity*/) const
{
	throw invalid_operation_error(
		"The Metal backend does not expose any memory resource yet."
	);
}

std::shared_ptr<xmipp4::command_queue> device::create_command_queue() const
{
	return std::make_shared<command_queue>(m_handle);
}

std::shared_ptr<xmipp4::event>
device::create_event(event_usage_flags /*usage*/) const
{
	return std::make_shared<event>(m_handle);
}

} // namespace metal
} // namespace xmipp4
