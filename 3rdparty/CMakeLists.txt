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


find_or_add(sophus
    GIT_REPOSITORY https://github.com/strasdat/Sophus.git
    GIT_TAG "main"
)

find_or_add(eigen
  GIT_REPOSITORY https://github.com/PX4/eigen.git
  CMAKE_CACHE_VARS
    "BUILD_TESTING=OFF"
    "EIGEN_BUILD_TESTING=OFF"
    "EIGEN_BUILD_BENCHMARKS=OFF"
    "EIGEN_BUILD_DOC=OFF"
    "EIGEN_BUILD_DEMOS=OFF"
    "EIGEN_BUILD_BLAS=OFF"
    "EIGEN_BUILD_LAPACK=OFF"
  POST_FETCH_COMMANDS
    "sed -i 's/trt\\.derived())/trt)/' Eigen/src/Core/Transpositions.h"
)
