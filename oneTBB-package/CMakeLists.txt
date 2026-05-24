include(FetchContent)

set(TBB_TEST OFF CACHE BOOL "" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    oneTBB
    GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(oneTBB)

add_library(tppbm_onetbb_package INTERFACE)
add_library(tppbm::onetbb ALIAS tppbm_onetbb_package)

if(TARGET TBB::tbb)
    set(TPPBM_ONETBB_TARGET TBB::tbb)
elseif(TARGET tbb)
    set(TPPBM_ONETBB_TARGET tbb)
else()
    message(FATAL_ERROR "oneTBB target was not found")
endif()

target_link_libraries(tppbm_onetbb_package
    INTERFACE
    ${TPPBM_ONETBB_TARGET}
)
