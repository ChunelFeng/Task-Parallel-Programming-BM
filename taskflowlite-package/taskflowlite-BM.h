
#ifndef TASKPARALLELPROGRAMMINGBM_TASKFLOWLITE_BM_H
#define TASKPARALLELPROGRAMMINGBM_TASKFLOWLITE_BM_H

#ifdef likely
#undef likely
#endif

#ifdef unlikely
#undef unlikely
#endif

#include "taskflowlite/taskflowlite.hpp"

#include <benchmark/benchmark.h>
#include <array>
#include <vector>

inline void taskflowlite_empty_function() {
}

void TASKFLOWLITE_32PARALLEL_8thds_BM(benchmark::State& state) {
    tfl::ResumeNever handler;
    tfl::Executor executor(handler, 8);
    tfl::Flow flow;

    const int size = 32;
    std::array<tfl::Task, size> arr;
    for (int i = 0; i < size; i++) {
        arr[i] = flow.emplace(taskflowlite_empty_function);
    }

    for (auto _ : state) {
        executor.async(flow).wait();
    }
}


void TASKFLOWLITE_32SERIAL_1thds_BM(benchmark::State& state) {
    tfl::ResumeNever handler;
    tfl::Executor executor(handler, 1);
    tfl::Flow flow;

    const int size = 32;
    std::array<tfl::Task, size> arr;

    arr[0] = flow.emplace(taskflowlite_empty_function);
    for (int i = 1; i < size; i++) {
        arr[i] = flow.emplace(taskflowlite_empty_function);
        arr[i - 1].precede(arr[i]);
    }

    for (auto _ : state) {
        executor.async(flow).wait();
    }
}


void TASKFLOWLITE_SIMPLE_2thds_BM(benchmark::State& state) {
    tfl::ResumeNever handler;
    tfl::Executor executor(handler, 2);
    tfl::Flow flow;

    auto a = flow.emplace(taskflowlite_empty_function);
    auto b1 = flow.emplace(taskflowlite_empty_function);
    auto b2 = flow.emplace(taskflowlite_empty_function);
    auto c1 = flow.emplace(taskflowlite_empty_function);
    auto c2 = flow.emplace(taskflowlite_empty_function);
    auto d = flow.emplace(taskflowlite_empty_function);

    a.precede(b1, c1);
    b1.precede(b2);
    c1.precede(c2);
    b2.precede(d);
    c2.precede(d);

    for (auto _ : state) {
        executor.async(flow).wait();
    }
}


void TASKFLOWLITE_8x8FC_8thds_BM(benchmark::State& state) {
    const int layer = 8;
    const int nodePerLayer = 8;

    tfl::ResumeNever handler;
    tfl::Executor executor(handler, nodePerLayer);
    tfl::Flow flow;

    std::vector<tfl::Task> beforeLayer;
    std::vector<tfl::Task> curLayer;
    beforeLayer.reserve(nodePerLayer);
    curLayer.reserve(nodePerLayer);

    for (int i = 0; i < layer; i++) {
        curLayer.clear();
        for (int j = 0; j < nodePerLayer; j++) {
            tfl::Task task = flow.emplace(taskflowlite_empty_function);
            for (auto& before : beforeLayer) {
                before.precede(task);
            }
            curLayer.emplace_back(task);
        }

        beforeLayer = curLayer;
    }

    for (auto _ : state) {
        executor.async(flow).wait();
    }
}

#endif //TASKPARALLELPROGRAMMINGBM_TASKFLOWLITE_BM_H
