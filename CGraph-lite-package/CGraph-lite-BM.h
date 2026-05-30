
#ifndef TASKPARALLELPROGRAMMINGBM_CGRAPH_LITE_BM_H
#define TASKPARALLELPROGRAMMINGBM_CGRAPH_LITE_BM_H

#include <benchmark/benchmark.h>
#include <memory>
#include <set>
#include <string>
#include <vector>

#define private public
#define protected public
#include "CGraph-lite.h"
#undef protected
#undef private

class CGraphLiteEmptyGNode : public CGraphLite::GNode {
public:
    CGraphLite::CStatus run() final {
        return CGraphLite::CStatus();
    }
};

inline void cgraphlite_init(CGraphLite::GPipeline* pipeline, size_t threadSize) {
    pipeline->status_ = CGraphLite::CStatus();
    for (auto* element : pipeline->elements_) {
        pipeline->status_ += element->init();
    }
    pipeline->schedule_ = std::make_unique<CGraphLite::Schedule>(threadSize);
}

inline void cgraphlite_destroy(CGraphLite::GPipeline* pipeline) {
    for (auto* element : pipeline->elements_) {
        pipeline->status_ += element->destroy();
    }
    pipeline->schedule_.reset();
}


void CGRAPHLITE_32PARALLEL_8thds_BM(benchmark::State& state) {
    CGraphLite::GPipeline* pipeline = CGraphLite::GPipelineFactory::create();
    const int size = 32;
    CGraphLite::GElement* arr[size] = {};

    for (auto& i : arr) {
        pipeline->registerGElement<CGraphLiteEmptyGNode>(&i, {}, "");
    }
    cgraphlite_init(pipeline, 8);

    for (auto _ : state) {
        pipeline->run();
    }

    cgraphlite_destroy(pipeline);
    CGraphLite::GPipelineFactory::remove(pipeline);
}


void CGRAPHLITE_32SERIAL_1thds_BM(benchmark::State& state) {
    CGraphLite::GPipeline* pipeline = CGraphLite::GPipelineFactory::create();
    const int size = 32;
    CGraphLite::GElement* arr[size];

    pipeline->registerGElement<CGraphLiteEmptyGNode>(&arr[0], {}, "");
    for (int i = 1; i < size; i++) {
        pipeline->registerGElement<CGraphLiteEmptyGNode>(&arr[i], {arr[i - 1]}, "");
    }
    cgraphlite_init(pipeline, 1);

    for (auto _ : state) {
        pipeline->run();
    }

    cgraphlite_destroy(pipeline);
    CGraphLite::GPipelineFactory::remove(pipeline);
}


void CGRAPHLITE_SIMPLE_2thds_BM(benchmark::State& state) {
    CGraphLite::GPipeline* pipeline = CGraphLite::GPipelineFactory::create();
    CGraphLite::GElement *a, *b1, *b2, *c1, *c2, *d = nullptr;

    pipeline->registerGElement<CGraphLiteEmptyGNode>(&a, {}, "");
    pipeline->registerGElement<CGraphLiteEmptyGNode>(&b1, {a}, "");
    pipeline->registerGElement<CGraphLiteEmptyGNode>(&b2, {b1}, "");
    pipeline->registerGElement<CGraphLiteEmptyGNode>(&c1, {a}, "");
    pipeline->registerGElement<CGraphLiteEmptyGNode>(&c2, {c1}, "");
    pipeline->registerGElement<CGraphLiteEmptyGNode>(&d, {b2, c2}, "");
    cgraphlite_init(pipeline, 2);

    for (auto _ : state) {
        pipeline->run();
    }

    cgraphlite_destroy(pipeline);
    CGraphLite::GPipelineFactory::remove(pipeline);
}


void CGRAPHLITE_8x8FC_8thds_BM(benchmark::State& state) {
    CGraphLite::GPipeline* pipeline = CGraphLite::GPipelineFactory::create();

    const int layer = 8;
    const int nodePerLayer = 8;

    std::set<CGraphLite::GElement*> curLayer {};
    std::set<CGraphLite::GElement*> beforeLayer {};
    for (int i = 0; i < layer; i++) {
        for (int j = 0; j < nodePerLayer; j++) {
            CGraphLite::GElement* ptr = nullptr;
            pipeline->registerGElement<CGraphLiteEmptyGNode>(&ptr, beforeLayer, "");
            curLayer.insert(ptr);
        }

        beforeLayer = curLayer;
        curLayer.clear();
    }

    cgraphlite_init(pipeline, nodePerLayer);
    for (auto _ : state) {
        pipeline->run();
    }
    cgraphlite_destroy(pipeline);
    CGraphLite::GPipelineFactory::remove(pipeline);
}


#endif //TASKPARALLELPROGRAMMINGBM_CGRAPH_LITE_BM_H
