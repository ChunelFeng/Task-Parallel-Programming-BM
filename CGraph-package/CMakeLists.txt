include(FetchContent)

set(CGRAPH_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
set(CGRAPH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CGRAPH_BUILD_FUNCTIONAL_TESTS OFF CACHE BOOL "" FORCE)
set(CGRAPH_BUILD_PERFORMANCE_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    CGraph
    GIT_REPOSITORY https://github.com/ChunelFeng/CGraph.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(CGraph)

add_library(tppbm_cgraph_package INTERFACE)
add_library(tppbm::cgraph ALIAS tppbm_cgraph_package)

target_include_directories(tppbm_cgraph_package
    INTERFACE
    ${cgraph_SOURCE_DIR}/src
)

target_sources(tppbm_cgraph_package
    INTERFACE
    $<TARGET_OBJECTS:CGraph>
)

target_link_libraries(tppbm_cgraph_package
    INTERFACE
    CGraph
)
