cmake_minimum_required(VERSION 3.25)

project(TaskParallelProgrammingBM LANGUAGES CXX)

add_subdirectory(CGraph-package)
add_subdirectory(CGraph-lite-package)
add_subdirectory(taskflow-package)
add_subdirectory(taskflowlite-package)
add_subdirectory(oneTBB-package)
add_subdirectory(scheduling-package)
add_subdirectory(benchmark-package)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tppbm::cgraph
    tppbm::cgraphlite
    tppbm::taskflow
    tppbm::taskflowlite
    tppbm::onetbb
    tppbm::scheduling
    tppbm::benchmark
)
