include(FetchContent)

set(TPPBM_BENCHMARK_SOURCE_DIR "" CACHE PATH "Path to local Google Benchmark source tree")

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

if(TPPBM_BENCHMARK_SOURCE_DIR)
    if(NOT EXISTS "${TPPBM_BENCHMARK_SOURCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "Invalid Google Benchmark source directory: ${TPPBM_BENCHMARK_SOURCE_DIR}")
    endif()

    add_subdirectory(
        "${TPPBM_BENCHMARK_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/benchmark"
    )
else()
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        UPDATE_DISCONNECTED TRUE
    )

    FetchContent_MakeAvailable(benchmark)
endif()

add_library(tppbm_benchmark_package INTERFACE)
add_library(tppbm::benchmark ALIAS tppbm_benchmark_package)

target_link_libraries(tppbm_benchmark_package
    INTERFACE
    benchmark::benchmark
)