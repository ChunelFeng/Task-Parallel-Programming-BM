include(FetchContent)

set(TASKFLOWLITE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TASKFLOWLITE_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(TASKFLOWLITE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TASKFLOWLITE_SANITIZER OFF CACHE STRING "" FORCE)

FetchContent_Declare(
    taskflowlite
    GIT_REPOSITORY https://github.com/wicyn/taskflowlite.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(taskflowlite)

add_library(tppbm_taskflowlite_package INTERFACE)
add_library(tppbm::taskflowlite ALIAS tppbm_taskflowlite_package)

target_link_libraries(tppbm_taskflowlite_package
    INTERFACE
    TaskflowLite::taskflowlite
)
