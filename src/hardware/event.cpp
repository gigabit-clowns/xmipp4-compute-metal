// SPDX-License-Identifier: GPL-3.0-only

#include "event.hpp"

#include <limits>
#include <stdexcept>

namespace xmipp4
{
namespace metal
{

event::event(const NS::SharedPtr<MTL::Device> &device)
	: m_event(NS::TransferPtr(device->newSharedEvent()))
	, m_target_value(0)
{
}

event::~event() = default;

event::handle event::get_handle() const noexcept
{
	return m_event.get();
}

event_usage_flags event::get_supported_usage() const noexcept
{
	return {
		event_usage_flag_bits::host_query,
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait,
		event_usage_flag_bits::cross_device_wait
	};
}

void event::wait() const
{
	const auto value = m_target_value.load();
	m_event->waitUntilSignaledValue(
		value,
		std::numeric_limits<std::uint64_t>::max()
	);
}

bool event::is_signaled() const
{
	return m_event->signaledValue() >= m_target_value.load();
}

std::uint64_t event::reserve_target_value() noexcept
{
	return m_target_value.fetch_add(1) + 1;
}

std::uint64_t event::get_target_value() const noexcept
{
	return m_target_value.load();
}

event& event::cast(xmipp4::event &ev)
{
	auto *result = dynamic_cast<event*>(&ev);
	if (!result)
	{
		throw std::invalid_argument(
			"The provided event was not created by the Metal backend."
		);
	}

	return *result;
}

const event& event::cast(const xmipp4::event &ev)
{
	const auto *result = dynamic_cast<const event*>(&ev);
	if (!result)
	{
		throw std::invalid_argument(
			"The provided event was not created by the Metal backend."
		);
	}

	return *result;
}

} // namespace metal
} // namespace xmipp4
