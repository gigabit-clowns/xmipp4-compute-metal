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
template<typename T>
inline void metal_check_not_null(
    T ptr,
    const char* call,
    const char* file,
    int line)
{
    if (!ptr)
    {
        std::string msg = std::string("Metal error: call '")
                        + call
                        + "' returned null ("
                        + file
                        + ":"
                        + std::to_string(line)
                        + ")";
        throw metal_error(msg);
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
