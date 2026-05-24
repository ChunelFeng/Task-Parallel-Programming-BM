
#include <benchmark/benchmark.h>

#include <string>
#include <vector>

#include "CGraph-package/CGraph-BM.h"
#include "taskflow-package/taskflow-BM.h"
#include "taskflowlite-package/taskflowlite-BM.h"
#include "oneTBB-package/oneTBB-BM.h"
#include "scheduling-package/scheduling-BM.h"

namespace {

class SeparatorConsoleReporter : public benchmark::ConsoleReporter {
public:
    using benchmark::ConsoleReporter::ConsoleReporter;

    void ReportRuns(const std::vector<benchmark::BenchmarkReporter::Run>& reports) override {
        if (!reports.empty()) {
            const std::string current_case = getCaseName(reports.front().benchmark_name());
            if (!last_case_.empty() && current_case != last_case_) {
                GetOutputStream() << "---------------------------------------------------------------------------\n";
            }
            last_case_ = current_case;
        }

        benchmark::ConsoleReporter::ReportRuns(reports);
    }

private:
    static std::string getCaseName(const std::string& name) {
        const std::size_t pos = name.find('_');
        if (pos == std::string::npos) {
            return name;
        }
        return name.substr(pos + 1);
    }

    std::string last_case_;
};

}  // namespace

BENCHMARK(CGRAPH_32PARALLEL_8thds_BM);
BENCHMARK(TASKFLOW_32PARALLEL_8thds_BM);
BENCHMARK(TASKFLOWLITE_32PARALLEL_8thds_BM);
BENCHMARK(ONETBB_32PARALLEL_8thds_BM);
BENCHMARK(SCHEDULING_32PARALLEL_8thds_BM);


BENCHMARK(CGRAPH_32SERIAL_1thds_BM);
BENCHMARK(TASKFLOW_32SERIAL_1thds_BM);
BENCHMARK(TASKFLOWLITE_32SERIAL_1thds_BM);
BENCHMARK(ONETBB_32SERIAL_1thds_BM);
BENCHMARK(SCHEDULING_32SERIAL_1thds_BM);


BENCHMARK(CGRAPH_SIMPLE_2thds_BM);
BENCHMARK(TASKFLOW_SIMPLE_2thds_BM);
BENCHMARK(TASKFLOWLITE_SIMPLE_2thds_BM);
BENCHMARK(ONETBB_SIMPLE_2thds_BM);
BENCHMARK(SCHEDULING_SIMPLE_2thds_BM);


BENCHMARK(CGRAPH_8x8FC_8thds_BM);
BENCHMARK(TASKFLOW_8x8FC_8thds_BM);
BENCHMARK(TASKFLOWLITE_8x8FC_8thds_BM);
BENCHMARK(ONETBB_8x8FC_8thds_BM);
BENCHMARK(SCHEDULING_8x8FC_8thds_BM);


int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }

    SeparatorConsoleReporter reporter;
    benchmark::RunSpecifiedBenchmarks(&reporter);
    benchmark::Shutdown();
    return 0;
}
