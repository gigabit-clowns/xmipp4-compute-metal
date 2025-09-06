// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/metal/compute/metal_error.hpp>

#include <sstream>

namespace xmipp4 
{
namespace compute 
{

void metal_check(NS::Error* error, 
                const char* call, 
                const char* file,
                int line)
{
    if (error) 
    {
        std::ostringstream oss;
        oss << "A METAL Runtime Error has occured: " << call << "\n"
            << " File: " << file << ":" << line << "\n"
            << " Reason: " << error->localizedDescription()->utf8String() 
            << std::endl;
        std::abort();
        throw metal_error(oss.str());
    }

}

} // namespace compute
} // namespace xmipp4