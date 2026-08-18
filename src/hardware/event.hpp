// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/event.hpp>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <atomic>
#include <cstdint>

namespace xmipp4
{
namespace metal
{

/**
 * @brief Metal implementation of @ref xmipp4::event, backed by a
 * MTL::SharedEvent.
 *
 * A MTL::SharedEvent signals by reaching a monotonically increasing 64 bit
 * value rather than a boolean state. Every @ref reserve_target_value call
 * reserves the next value; it is what @ref command_queue::signal encodes on
 * the command buffer, and what subsequent waits and queries check against.
 *
 * Supports every @ref event_usage_flag_bits value: a MTL::SharedEvent can be
 * signaled and waited on by any queue on the owning device, waited on and
 * queried from the host, and shared with other devices through a
 * MTL::SharedEventHandle.
 */
class event final
	: public xmipp4::event
{
public:
	using handle = MTL::SharedEvent*;

	explicit event(const NS::SharedPtr<MTL::Device> &device);
	event(const event &other) = delete;
	event(event &&other) = delete;
	~event() override;

	event& operator=(const event &other) = delete;
	event& operator=(event &&other) = delete;

	handle get_handle() const noexcept;

	event_usage_flags get_supported_usage() const noexcept override;

	void wait() const override;
	bool is_signaled() const override;

	/**
	 * @brief Reserve the next signal value for this event.
	 *
	 * @return The value that must be encoded on the command buffer that
	 * signals this event.
	 */
	std::uint64_t reserve_target_value() noexcept;

	/**
	 * @brief Get the most recently reserved signal value.
	 */
	std::uint64_t get_target_value() const noexcept;

	/**
	 * @brief Downcast an event to this backend's implementation.
	 *
	 * @param ev The event to be cast.
	 * @return event& The same event, as a Metal event.
	 *
	 * @throws std::invalid_argument If the event was not created by this
	 * backend.
	 */
	static event& cast(xmipp4::event &ev);

	/**
	 * @brief Downcast an event to this backend's implementation.
	 *
	 * @param ev The event to be cast.
	 * @return const event& The same event, as a Metal event.
	 *
	 * @throws std::invalid_argument If the event was not created by this
	 * backend.
	 */
	static const event& cast(const xmipp4::event &ev);

private:
	NS::SharedPtr<MTL::SharedEvent> m_event;
	std::atomic<std::uint64_t> m_target_value;
};

} // namespace metal
} // namespace xmipp4
