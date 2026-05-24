#ifndef TASKPARALLELPROGRAMMINGBM_ONETBB_BM_H
#define TASKPARALLELPROGRAMMINGBM_ONETBB_BM_H

#include <benchmark/benchmark.h>
#include <oneapi/tbb/flow_graph.h>
#include <oneapi/tbb/global_control.h>

#include <array>
#include <memory>
#include <vector>

inline void onetbb_empty_function() {
}

using OneTBBContinueMsg = oneapi::tbb::flow::continue_msg;
using OneTBBContinueNode = oneapi::tbb::flow::continue_node<OneTBBContinueMsg>;

inline std::unique_ptr<OneTBBContinueNode> make_onetbb_empty_node(oneapi::tbb::flow::graph& graph) {
    return std::make_unique<OneTBBContinueNode>(
        graph,
        [](const OneTBBContinueMsg&) {
            onetbb_empty_function();
        }
    );
}

void ONETBB_32PARALLEL_8thds_BM(benchmark::State& state) {
    oneapi::tbb::global_control control(oneapi::tbb::global_control::max_allowed_parallelism, 8);
    oneapi::tbb::flow::graph graph;

    const int size = 32;
    std::array<std::unique_ptr<OneTBBContinueNode>, size> arr;
    for (int i = 0; i < size; i++) {
        arr[i] = make_onetbb_empty_node(graph);
    }

    for (auto _ : state) {
        for (auto& node : arr) {
            node->try_put(OneTBBContinueMsg{});
        }
        graph.wait_for_all();
    }
}


void ONETBB_32SERIAL_1thds_BM(benchmark::State& state) {
    oneapi::tbb::global_control control(oneapi::tbb::global_control::max_allowed_parallelism, 1);
    oneapi::tbb::flow::graph graph;

    const int size = 32;
    std::array<std::unique_ptr<OneTBBContinueNode>, size> arr;
    for (int i = 0; i < size; i++) {
        arr[i] = make_onetbb_empty_node(graph);
    }
    for (int i = 1; i < size; i++) {
        oneapi::tbb::flow::make_edge(*arr[i - 1], *arr[i]);
    }

    for (auto _ : state) {
        arr[0]->try_put(OneTBBContinueMsg{});
        graph.wait_for_all();
    }
}


void ONETBB_SIMPLE_2thds_BM(benchmark::State& state) {
    oneapi::tbb::global_control control(oneapi::tbb::global_control::max_allowed_parallelism, 2);
    oneapi::tbb::flow::graph graph;

    auto a = make_onetbb_empty_node(graph);
    auto b1 = make_onetbb_empty_node(graph);
    auto b2 = make_onetbb_empty_node(graph);
    auto c1 = make_onetbb_empty_node(graph);
    auto c2 = make_onetbb_empty_node(graph);
    auto d = make_onetbb_empty_node(graph);

    oneapi::tbb::flow::make_edge(*a, *b1);
    oneapi::tbb::flow::make_edge(*a, *c1);
    oneapi::tbb::flow::make_edge(*b1, *b2);
    oneapi::tbb::flow::make_edge(*c1, *c2);
    oneapi::tbb::flow::make_edge(*b2, *d);
    oneapi::tbb::flow::make_edge(*c2, *d);

    for (auto _ : state) {
        a->try_put(OneTBBContinueMsg{});
        graph.wait_for_all();
    }
}


void ONETBB_8x8FC_8thds_BM(benchmark::State& state) {
    const int layer = 8;
    const int nodePerLayer = 8;

    oneapi::tbb::global_control control(oneapi::tbb::global_control::max_allowed_parallelism, nodePerLayer);
    oneapi::tbb::flow::graph graph;

    std::vector<std::unique_ptr<OneTBBContinueNode>> nodes;
    nodes.reserve(layer * nodePerLayer);

    std::vector<OneTBBContinueNode*> beforeLayer;
    std::vector<OneTBBContinueNode*> curLayer;
    beforeLayer.reserve(nodePerLayer);
    curLayer.reserve(nodePerLayer);

    for (int i = 0; i < layer; i++) {
        curLayer.clear();
        for (int j = 0; j < nodePerLayer; j++) {
            nodes.emplace_back(make_onetbb_empty_node(graph));
            OneTBBContinueNode* node = nodes.back().get();
            for (auto* before : beforeLayer) {
                oneapi::tbb::flow::make_edge(*before, *node);
            }
            curLayer.emplace_back(node);
        }

        beforeLayer = curLayer;
    }

    for (auto _ : state) {
        for (int i = 0; i < nodePerLayer; i++) {
            nodes[i]->try_put(OneTBBContinueMsg{});
        }
        graph.wait_for_all();
    }
}

#endif //TASKPARALLELPROGRAMMINGBM_ONETBB_BM_H
