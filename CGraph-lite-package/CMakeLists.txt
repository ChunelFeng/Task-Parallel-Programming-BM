include(FetchContent)

FetchContent_Declare(
    CGraphLite
    GIT_REPOSITORY https://github.com/ChunelFeng/CGraph-lite.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_CONFIG http.proxy= https.proxy=
    UPDATE_DISCONNECTED TRUE
)

FetchContent_GetProperties(CGraphLite)
if (NOT cgraphlite_POPULATED)
    FetchContent_Populate(CGraphLite)
endif ()

add_library(tppbm_cgraphlite_package INTERFACE)
add_library(tppbm::cgraphlite ALIAS tppbm_cgraphlite_package)

target_include_directories(tppbm_cgraphlite_package
    INTERFACE
    ${cgraphlite_SOURCE_DIR}
)
