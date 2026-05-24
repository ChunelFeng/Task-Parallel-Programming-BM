include(FetchContent)

set(TF_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TF_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TF_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(TF_BUILD_PROFILER OFF CACHE BOOL "" FORCE)
set(TF_BUILD_CUDA OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    taskflow
    GIT_REPOSITORY https://github.com/taskflow/taskflow.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(taskflow)

add_library(tppbm_taskflow_package INTERFACE)
add_library(tppbm::taskflow ALIAS tppbm_taskflow_package)

target_include_directories(tppbm_taskflow_package
    INTERFACE
    ${taskflow_SOURCE_DIR}
)
