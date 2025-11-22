// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

#include "../dynamic_shared_object.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Exception class representing a Metal runtime error.
 * 
 */
class metal_error
    : public std::runtime_error
{
    using runtime_error::runtime_error;
};

/**
 * @brief Check that a Metal object creation call returned a non-null pointer.
 *
 * Metal does not return error codes, so we treat `nullptr` as failure.
 */
XMIPP4_HARDWARE_METAL_API
inline void metal_check_not_null(
    const void* ptr,
    const char* call,
    const char* file,
    int line)
{
    if (!ptr)
    {
        char buf[1024];
        snprintf(buf, sizeof(buf),
                 "Metal error: call '%s' returned null (%s:%d)",
                 call, file, line);
        throw metal_error(buf);
    }
}

/**
 * @brief Macro equivalent to XMIPP4_CUDA_CHECK for Metal.
 *
 * Checks that the expression is not null, and throws a metal_error otherwise.
 */
#define XMIPP4_METAL_CHECK(expr) \
    ::xmipp4::hardware::metal_check_not_null((expr), #expr, __FILE__, __LINE__)

} // namespace hardware
} // namespace xmipp4
