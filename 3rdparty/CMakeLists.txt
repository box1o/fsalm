cmake_minimum_required(VERSION 4.2.0)

find_or_add(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

find_or_add(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

find_or_add(yaml
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
)


find_or_add(cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  CMAKE_CACHE_VARS
    "CXXOPTS_STANDALONE_PROJECT=OFF"
)


#NOTE: Graphics
find_or_add(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
    DISABLE_INSTALL
)

# find_or_add(tinygltf
#     GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
#     GIT_TAG release
# )

add_subdirectory(stb)
add_subdirectory(glad)

find_or_add(spirv-cross
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG main 
)

find_or_add(shaderc
    GIT_REPOSITORY https://github.com/google/shaderc.git
    GIT_TAG main
    RECURSE_SUBMODULES
    DISABLE_INSTALL
    POST_FETCH_SCRIPT utils/git-sync-deps
    CMAKE_CACHE_VARS
        SHADERC_SKIP_TESTS=ON
        SHADERC_SKIP_INSTALL=ON
        SHADERC_SKIP_EXAMPLES=ON
        SHADERC_SKIP_COPYRIGHT_CHECK=ON
)


#NOTE: Math related
# find_or_add(sophus
#     GIT_REPOSITORY https://github.com/strasdat/Sophus.git
#     GIT_TAG "main"
# )
