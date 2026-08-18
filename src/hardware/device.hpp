// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace metal
{

/**
 * @brief Metal implementation of @ref xmipp4::device.
 *
 * The handle is a thin wrapper around a retained MTL::Device: it owns no
 * other driver resource, so several handles may refer to the same physical
 * device without interfering with each other.
 */
class device final
	: public xmipp4::device
{
public:
	explicit device(NS::SharedPtr<MTL::Device> handle) noexcept;
	device(const device &other) = delete;
	device(device &&other) = delete;
	~device() override;

	device& operator=(const device &other) = delete;
	device& operator=(device &&other) = delete;

	const NS::SharedPtr<MTL::Device>& get_handle() const noexcept;

	const memory_resource&
	get_memory_resource(memory_resource_affinity affinity) const override;

	std::shared_ptr<xmipp4::command_queue> create_command_queue() const override;

	std::shared_ptr<xmipp4::event>
	create_event(event_usage_flags usage) const override;

private:
	NS::SharedPtr<MTL::Device> m_handle;
};

} // namespace metal
} // namespace xmipp4
