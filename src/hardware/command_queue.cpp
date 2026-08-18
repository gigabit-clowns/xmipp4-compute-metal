// SPDX-License-Identifier: GPL-3.0-only

#include "command_queue.hpp"

#include "event.hpp"

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/hardware/command.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace metal
{

command_queue::command_queue(const NS::SharedPtr<MTL::Device> &device)
	: m_queue(NS::TransferPtr(device->newCommandQueue()))
{
}

command_queue::~command_queue() = default;

command_queue::handle command_queue::get_handle() const noexcept
{
	return m_queue.get();
}

void command_queue::submit(const command &cmd)
{
	if (!cmd.get_program())
	{
		throw std::invalid_argument(
			"command_queue::submit: Provided command does not have an "
			"associated program."
		);
	}

	throw invalid_operation_error(
		"The Metal backend does not implement any program yet."
	);
}

void command_queue::signal(xmipp4::event &ev)
{
	auto &metal_event = event::cast(ev);
	const auto value = metal_event.reserve_target_value();

	// Metal has no queue-level signal operation; the signal is encoded on a
	// command buffer of its own and committed right away.
	auto cmd_buffer = NS::RetainPtr(m_queue->commandBuffer());
	cmd_buffer->encodeSignalEvent(metal_event.get_handle(), value);
	cmd_buffer->commit();
}

void command_queue::wait(const xmipp4::event &ev)
{
	const auto &metal_event = event::cast(ev);

	auto cmd_buffer = NS::RetainPtr(m_queue->commandBuffer());
	cmd_buffer->encodeWait(metal_event.get_handle(), metal_event.get_target_value());
	cmd_buffer->commit();
}

} // namespace metal
} // namespace xmipp4
