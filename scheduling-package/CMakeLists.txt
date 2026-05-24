include(FetchContent)

set(SCHEDULING_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(SCHEDULING_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(SCHEDULING_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SCHEDULING_BUILD_TASKFLOW_BENCHMARKS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    scheduling
    GIT_REPOSITORY https://github.com/dpuyda/scheduling.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(scheduling)

add_library(tppbm_scheduling_package INTERFACE)
add_library(tppbm::scheduling ALIAS tppbm_scheduling_package)

target_link_libraries(tppbm_scheduling_package
    INTERFACE
    scheduling
)
