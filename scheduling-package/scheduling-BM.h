#ifndef TASKPARALLELPROGRAMMINGBM_SCHEDULING_BM_H
#define TASKPARALLELPROGRAMMINGBM_SCHEDULING_BM_H

#include <benchmark/benchmark.h>
#include <scheduling/scheduling.hpp>

#include <array>
#include <vector>

inline void scheduling_empty_function() {
}

void SCHEDULING_32PARALLEL_8thds_BM(benchmark::State& state) {
    scheduling::ThreadPool thread_pool(8);

    const int size = 32;
    std::array<scheduling::Task, size> arr;
    for (int i = 0; i < size; i++) {
        arr[i] = scheduling::Task(scheduling_empty_function);
    }

    for (auto _ : state) {
        thread_pool.Submit(arr);
        thread_pool.Wait();
    }
}


void SCHEDULING_32SERIAL_1thds_BM(benchmark::State& state) {
    scheduling::ThreadPool thread_pool(1);

    const int size = 32;
    std::array<scheduling::Task, size> arr;

    arr[0] = scheduling::Task(scheduling_empty_function);
    for (int i = 1; i < size; i++) {
        arr[i] = scheduling::Task(scheduling_empty_function);
        arr[i - 1].Precede(&arr[i]);
    }

    for (auto _ : state) {
        thread_pool.Submit(arr);
        thread_pool.Wait();
    }
}


void SCHEDULING_SIMPLE_2thds_BM(benchmark::State& state) {
    scheduling::ThreadPool thread_pool(2);

    std::array<scheduling::Task, 6> arr;
    auto& a = arr[0] = scheduling::Task(scheduling_empty_function);
    auto& b1 = arr[1] = scheduling::Task(scheduling_empty_function);
    auto& b2 = arr[2] = scheduling::Task(scheduling_empty_function);
    auto& c1 = arr[3] = scheduling::Task(scheduling_empty_function);
    auto& c2 = arr[4] = scheduling::Task(scheduling_empty_function);
    auto& d = arr[5] = scheduling::Task(scheduling_empty_function);

    a.Precede(&b1, &c1);
    b1.Precede(&b2);
    c1.Precede(&c2);
    b2.Precede(&d);
    c2.Precede(&d);

    for (auto _ : state) {
        thread_pool.Submit(arr);
        thread_pool.Wait();
    }
}


void SCHEDULING_8x8FC_8thds_BM(benchmark::State& state) {
    const int layer = 8;
    const int nodePerLayer = 8;

    scheduling::ThreadPool thread_pool(nodePerLayer);

    std::vector<scheduling::Task> tasks;
    tasks.reserve(layer * nodePerLayer);

    std::vector<scheduling::Task*> beforeLayer;
    std::vector<scheduling::Task*> curLayer;
    beforeLayer.reserve(nodePerLayer);
    curLayer.reserve(nodePerLayer);

    for (int i = 0; i < layer; i++) {
        curLayer.clear();
        for (int j = 0; j < nodePerLayer; j++) {
            tasks.emplace_back(scheduling_empty_function);
            scheduling::Task* task = &tasks.back();
            for (auto* before : beforeLayer) {
                before->Precede(task);
            }
            curLayer.emplace_back(task);
        }

        beforeLayer = curLayer;
    }

    for (auto _ : state) {
        thread_pool.Submit(tasks);
        thread_pool.Wait();
    }
}

#endif //TASKPARALLELPROGRAMMINGBM_SCHEDULING_BM_H
