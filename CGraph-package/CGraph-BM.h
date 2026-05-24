
#ifndef TASKPARALLELPROGRAMMINGBM_CGRAPH_BM_H
#define TASKPARALLELPROGRAMMINGBM_CGRAPH_BM_H

#include "CGraph.h"

#include <benchmark/benchmark.h>

using namespace CGraph;

class EmptyGNode : public GNode {
public:
    CStatus run() final {
        return CStatus();
    }
};


void CGRAPH_32PARALLEL_8thds_BM(benchmark::State& state) {
    GPipelinePtr pipeline = GPipelineFactory::create();
    const int size = 32;
    GElementPtr arr[size] = {};

    UThreadPoolConfig config;
    config.default_thread_size_ = 8;
    config.secondary_thread_size_ = 0;
    config.max_task_steal_range_ = 7;
    config.max_thread_size_ = 8;
    config.primary_thread_empty_interval_ = 0;
    config.primary_thread_busy_epoch_ = 500000;
    pipeline->setUniqueThreadPoolConfig(config);
    for (auto& i : arr) {
        pipeline->registerGElement<EmptyGNode>(&i);
    }
    pipeline->init();

    for (auto _ : state) {
        pipeline->run();
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


void CGRAPH_32SERIAL_1thds_BM(benchmark::State& state) {
    GPipelinePtr pipeline = GPipelineFactory::create();
    const int size = 32;
    GElementPtr arr[size];

    pipeline->registerGElement<EmptyGNode>(&arr[0]);
    for (int i = 1; i < size; i++) {
        pipeline->registerGElement<EmptyGNode>(&arr[i], {arr[i - 1]});
    }
    pipeline->init();

    for (auto _ : state) {
        pipeline->run();
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


void CGRAPH_SIMPLE_2thds_BM(benchmark::State& state) {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a,b1,b2,c1,c2,d = nullptr;

    UThreadPoolConfig config;
    config.default_thread_size_ = 2;
    config.secondary_thread_size_ = 0;
    config.max_task_steal_range_ = 2;
    config.max_thread_size_ = 2;
    config.primary_thread_empty_interval_ = 0;
    config.primary_thread_busy_epoch_ = 500000;
    pipeline->setUniqueThreadPoolConfig(config);
    pipeline->registerGElement<EmptyGNode>(&a);
    pipeline->registerGElement<EmptyGNode>(&b1, {a});
    pipeline->registerGElement<EmptyGNode>(&b2, {b1});
    pipeline->registerGElement<EmptyGNode>(&c1, {a});
    pipeline->registerGElement<EmptyGNode>(&c2, {c1});
    pipeline->registerGElement<EmptyGNode>(&d, {b2, c2});
    pipeline->init();

    for (auto _ : state) {
        pipeline->run();
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


void CGRAPH_8x8FC_8thds_BM(benchmark::State& state) {
    GPipelinePtr pipeline = GPipelineFactory::create();

    const int layer = 8;
    const int nodePerLayer = 8;

    UThreadPoolConfig config;
    config.default_thread_size_ = nodePerLayer;
    config.max_thread_size_ = nodePerLayer;
    config.max_task_steal_range_ = nodePerLayer - 1;
    config.primary_thread_busy_epoch_ = 500000;
    config.primary_thread_empty_interval_ = 0;
    pipeline->setUniqueThreadPoolConfig(config);

    GElementPtrSet curLayer {};
    GElementPtrSet beforeLayer {};
    for (int i = 0; i < layer; i++) {
        for (int j = 0; j < nodePerLayer; j++) {
            GElementPtr ptr = nullptr;
            pipeline->registerGElement<EmptyGNode>(&ptr, beforeLayer);
            curLayer.insert(ptr);
        }

        beforeLayer = curLayer;
        curLayer.clear();
    }

    pipeline->init();
    for (auto _ : state) {
        pipeline->run();
    }
    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


#endif //TASKPARALLELPROGRAMMINGBM_CGRAPH_BM_H
