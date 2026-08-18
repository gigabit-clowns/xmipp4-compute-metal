// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4
{
namespace metal
{

/**
 * @brief Metal implementation of @ref xmipp4::command_queue, backed by a
 * MTL::CommandQueue.
 */
class command_queue final
	: public xmipp4::command_queue
{
public:
	using handle = MTL::CommandQueue*;

	explicit command_queue(const NS::SharedPtr<MTL::Device> &device);
	command_queue(const command_queue &other) = delete;
	command_queue(command_queue &&other) = delete;
	~command_queue() override;

	command_queue& operator=(const command_queue &other) = delete;
	command_queue& operator=(command_queue &&other) = delete;

	handle get_handle() const noexcept;

	void submit(const command &cmd) override;
	void signal(xmipp4::event &ev) override;
	void wait(const xmipp4::event &ev) override;

private:
	NS::SharedPtr<MTL::CommandQueue> m_queue;
};

} // namespace metal
} // namespace xmipp4
