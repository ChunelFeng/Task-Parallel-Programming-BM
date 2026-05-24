
#ifndef TASKPARALLELPROGRAMMINGBM_TASKFLOW_BM_H
#define TASKPARALLELPROGRAMMINGBM_TASKFLOW_BM_H

#include "taskflow/taskflow.hpp"

#include <benchmark/benchmark.h>
#include <array>
#include <vector>


inline void empty_function() {
}

void TASKFLOW_32PARALLEL_8thds_BM(benchmark::State& state) {
    tf::Executor executor(8);
    tf::Taskflow taskflow;

    const int size = 32;
    for (int i = 0; i < size; i++) {
        taskflow.emplace(empty_function);
    }

    for (auto _ : state) {
        executor.run(taskflow).wait();
    }
}


void TASKFLOW_32SERIAL_1thds_BM(benchmark::State& state) {
    tf::Executor executor(1);
    tf::Taskflow taskflow;

    const int size = 32;
    std::array<tf::Task, size> arr;

    arr[0] = taskflow.emplace(empty_function);
    for (int i = 1; i < size; i++) {
        arr[i] = taskflow.emplace(empty_function);
        arr[i - 1].precede(arr[i]);
    }

    for (auto _ : state) {
        executor.run(taskflow).wait();
    }
}


void TASKFLOW_SIMPLE_2thds_BM(benchmark::State& state) {
    tf::Executor executor(2);
    tf::Taskflow taskflow;

    auto a = taskflow.emplace(empty_function);
    auto b1 = taskflow.emplace(empty_function);
    auto b2 = taskflow.emplace(empty_function);
    auto c1 = taskflow.emplace(empty_function);
    auto c2 = taskflow.emplace(empty_function);
    auto d = taskflow.emplace(empty_function);

    a.precede(b1, c1);
    b1.precede(b2);
    c1.precede(c2);
    b2.precede(d);
    c2.precede(d);

    for (auto _ : state) {
        executor.run(taskflow).wait();
    }
}


void TASKFLOW_8x8FC_8thds_BM(benchmark::State& state) {
    const int layer = 8;
    const int nodePerLayer = 8;

    tf::Executor executor(nodePerLayer);
    tf::Taskflow taskflow;

    std::vector<tf::Task> beforeLayer;
    std::vector<tf::Task> curLayer;
    beforeLayer.reserve(nodePerLayer);
    curLayer.reserve(nodePerLayer);

    for (int i = 0; i < layer; i++) {
        curLayer.clear();
        for (int j = 0; j < nodePerLayer; j++) {
            tf::Task task = taskflow.emplace(empty_function);
            for (auto& before : beforeLayer) {
                before.precede(task);
            }
            curLayer.emplace_back(task);
        }

        beforeLayer = curLayer;
    }

    for (auto _ : state) {
        executor.run(taskflow).wait();
    }
}

#endif //TASKPARALLELPROGRAMMINGBM_TASKFLOW_BM_H
