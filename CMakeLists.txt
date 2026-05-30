cmake_minimum_required(VERSION 3.25)

project(TaskParallelProgrammingBM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(CGraph-package)
add_subdirectory(CGraph-lite-package)
add_subdirectory(taskflow-package)
add_subdirectory(taskflowlite-package)
add_subdirectory(oneTBB-package)
add_subdirectory(scheduling-package)

find_package(benchmark REQUIRED)


add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tppbm::cgraph
    tppbm::cgraphlite
    tppbm::taskflow
    tppbm::taskflowlite
    tppbm::onetbb
    tppbm::scheduling
    benchmark::benchmark
)
