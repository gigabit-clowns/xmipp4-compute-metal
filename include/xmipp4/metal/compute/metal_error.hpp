// SPDX-License-Identifier: GPL-3.0-only

#pragma once
#include <Metal/Metal.hpp>
#include <Foundation/Foundation.h>
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
                const char* expr, 
                const char* file,
                int line );

/**
 * @brief Calls metal_check filling the call name, filename and line number.
 * 
 */
#define XMIPP4_METAL_CHECK(EXPR)                        \
    [&]() {                                             \
        NS::Error* __err = nullptr;                     \
        auto __result = (EXPR);                         \
        metal_check(__err, #EXPR, __FILE__, __LINE__);  \
        return __result;                                \
    }()

} // namespace compute
} // namespace xmipp4