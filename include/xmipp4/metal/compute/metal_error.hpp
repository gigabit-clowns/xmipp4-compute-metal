// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <stdexcept>

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Exception class representing a METAL runtime error.
 * 
 */
class metal_error
    : public std::runtime_error
{
    using runtime_error::runtime_error;
};

/**
 * @brief Check METAL return code and throw an exception on failure.
 * 
 * @param error metal-cpp handle to NSError object.
 * @param expr String identifying the METAL function call.
 * @param file File where the error ocurred.
 * @param line Line where the error ocurred.
 * 
 */
void metal_check(NS::Error* error, 
                const char* call, 
                const char* file,
                int line );

/**
 * @brief Calls metal_check filling the call name, filename and line number.
 * This should not be used with void-returning functions: release, retain, autorelease
 * buffer commits, etc
 */
#define XMIPP4_METAL_CHECK(expr) \
    ([&]() -> decltype(expr) { \
        NS::Error* _mtl_err = nullptr; \
        auto _mtl_res = (expr); \
        metal_check(_mtl_err, #expr, __FILE__, __LINE__); \
        return _mtl_res; \
    }())

} // namespace compute
} // namespace xmipp4