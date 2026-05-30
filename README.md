# Task Parallel Programming Benchmark

This project is a C++ benchmark suite for comparing the scheduling overhead of mainstream open-source DAG/task scheduling frameworks under a small set of specific scenarios.

The goal is not to prove that one framework is universally better than another. Different runtimes are designed with different tradeoffs, APIs, execution models, and observability costs. This benchmark focuses on controlled DAG scheduling scenarios and provides data for reference.

## Benchmark Scope

The current benchmark cases include:

- 32 independent parallel tasks
- 32 serially dependent tasks
- A simple fork-join DAG
- An 8-layer fully connected DAG with 8 nodes per layer

Each framework should implement the same topology and roughly the same task body so that the measured result mainly reflects scheduling behavior.

## Current Frameworks

The project currently includes benchmark implementations for:

- [CGraph](https://github.com/ChunelFeng/CGraph)
- [CGraph-lite](https://github.com/ChunelFeng/CGraph-lite)
- [Taskflow](https://github.com/taskflow/taskflow)
- [TaskflowLite](https://github.com/wicyn/taskflowlite)
- [oneTBB](https://github.com/oneapi-src/oneTBB)
- [scheduling](https://github.com/dpuyda/scheduling)

More DAG/task scheduling frameworks are welcome.

## Contribution

Contributions are welcome at any time.

You are encouraged to submit benchmark results from different machines, operating systems, compilers, standard libraries, and build configurations. These comparisons are useful because scheduling behavior can vary significantly across environments.

When adding a new DAG scheduling framework, please make sure:

- The framework has its own package directory and CMake integration.
- The framework provides corresponding benchmark implementations.
- The benchmark function names, topology, task count, and output style stay consistent with the existing benchmark cases.
- The benchmark code is easy to read, debug, and extend.

## Benchmark Rules

Framework-specific configuration is allowed, including scheduler options, queue configuration, memory-related options, or runtime tuning parameters.

However:

- The number of worker threads must remain consistent with the existing benchmark cases.
- Thread parameters must not be modified, including priority, affinity, scheduling policy, or other OS-level thread behavior.
- The benchmark topology must not be changed to favor a specific framework.
- The benchmark task body should stay functionally equivalent across frameworks.

These rules are intended to keep the comparison focused on DAG scheduling behavior instead of external thread tuning.

## Result Disclaimer

Benchmark results can differ significantly across environments.

CPU model, core count, operating system, compiler version, optimization flags, standard library implementation, system load, power mode, and background services may all affect the final numbers.

Therefore, all benchmark data in this project should be treated as reference only, not as an absolute ranking.
