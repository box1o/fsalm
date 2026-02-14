#pragma once
#include <string>
#include <webgpu/webgpu_cpp.h>

namespace ct {

inline auto dsv = [](wgpu::StringView sv) -> std::string {
    if (!sv.data || sv.length == 0) return {};
    return std::string(sv.data, sv.length);
};

} // namespace ct
