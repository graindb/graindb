# GrainDB

# Requirements

GrainDB requires [CMake](https://cmake.org) to be installed and a `C++11` compliant compiler. GCC 4.9 and newer, Clang
3.9 and newer and VisualStudio 2017 are tested on each revision.

## Compiling

Run `make` in the root directory to compile the sources. For development, use `make debug` to build a non-optimized
debug version. You may run `make unit` and `make allunit` to verify that your version works properly after making
changes.

# Experiments
## Datasets
- **JOB**: third_party/imdb/data/
- **LDBC10**: third_party/ldbc/data/sf10/
- **LDBC30**: third_party/ldbc/data/sf30/

## End-To-End Benchmarks

### JOB: Relational Workload with Selective Many-to-Many Joins (8.2.1)
#### Run JOB evaluations on DuckDB and GRainDB:
```shell
> ./build/release/benchmark/benchmark_runner "IMDB_113_Q[0-9][0-9][0-9]" --out=evaluations/job_duckdb.out
> ./build/release/benchmark/benchmark_runner "IMDB_113_Q[0-9][0-9][0-9]A" --out=evaluations/job_graindb.out
```

#### Processing of query evaluation results:
```shell
> python3 scripts/extract_query_latency.py evaluations/job_duckdb.out evaluations/job_duckdb_avg.out
> python3 scripts/extract_query_latency.py evaluations/job_graindb.out evaluations/job_graindb_avg.out
```

### SNB-M: Graph Workload with Selective Many-to-Many Joins (8.2.2 & 8.2.3)
#### Run JOB evaluations on DuckDB, DuckDB-MV and GRainDB:
```shell
> ./build/release/benchmark/benchmark_runner "LDBC_LIGHT_OPTIMIZED_Q[0-9][0-9][0-9]" --out=evaluations/snb_duckdb.out
> ./build/release/benchmark/benchmark_runner "LDBC_LIGHT_VIEWS_.*" --out=evaluations/duckdb_mv.out
> ./build/release/benchmark/benchmark_runner "LDBC_LIGHT_OPTIMIZED_Q[0-9][0-9][0-9]A" --out=evaluations/snb_graindb.out
```

#### Processing of query evaluation results:
```shell
> python3 scripts/extract_query_latency.py evaluations/job_duckdb.out evaluations/job_duckdb_avg.out
> python3 scripts/extract_query_latency.py evaluations/duckdb_mv.out evaluations/duckdb_mv_avg.out
> python3 scripts/extract_query_latency.py evaluations/job_graindb.out evaluations/job_graindb_avg.out
```

#### Run GraphflowDB on SNB-M:
```shell
>
```

### TPC-H: Traditional OLAP Workloads (8.2.4)
#### Run TPC-H evaluations on DuckDB and GRainDB:
```shell
> ./build/release/benchmark/benchmark_runner "TPCH_OPTIMIZED_Q[0-9][0-9]" --out=evaluations/tpch_duckdb.out
> ./build/release/benchmark/benchmark_runner "TPCH_OPTIMIZED_Q[0-9][0-9]A" --out=evaluations/tpch_graindb.out
```

#### Processing of query evaluation results:
```shell
> python3 scripts/extract_query_latency.py evaluations/tpch_duckdb.out evaluations/tpch_duckdb_avg.out
> python3 scripts/extract_query_latency.py evaluations/tpch_graindb.out evaluations/tpch_graindb_avg.out
```

## Detailed Evaluation
### Ablation Study (8.3.1)
Configurations for GRainDB:
- GR-FULL. Set both `ENABLE_RAI_JOIN_MERGE` and `ENABLE_ALISTS` to `true`.
- GR-JM. Set `ENABLE_RAI_JOIN_MERGE` to `false`, and keep `ENABLE_ALISTS` to `true`.
- GR-JM-RSJ. Set both `ENABLE_RAI_JOIN_MERGE` and `ENABLE_ALISTS` to `false`.

Under each configuration, run following command to get GRainDB evaluation result on SNB-M under that configuration:
```shell
> ./build/release/benchmark/benchmark_runner "LDBC_LIGHT_OPTIMIZED_Q[0-9][0-9][0-9]A" --out=evaluations/snb_graindb_configuration_x.out
```

### Performance of Predefined Joins Under Varying Entity vs Relationship Table Selectivity (8.3.2)
- DuckDB and GRainDB
```shell
> ./build/release/benchmark/benchmark_runner
```

- GraphflowDB
```shell
>
```

- Neo4j
```shell
> 
```

### Plan Spectrum Analyses (8.3.3)

```shell
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q01/ 1 1 225
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q02/ 5 1 225
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q03/ 9 1 41
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q04/ 12 1 225
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q05/ 15 1 225
> ./build/release/examples/spectrum_runner/spectrum_runner spectrum_jos/Q06/ 18 1 225
```

### Plotting
#### Boxplot
- End to end benchmarks: JOB, SNB-M, TPC-H
```shell
> python3 scripts/plot_boxplot_job.py evaluations/job.csv
> python3 scripts/plot_boxplot_snb.py evaluations/snb.csv
> python3 scripts/plot_boxplot_tpch.py evaluations/tpch.csv
```

- Ablation
```shell
> python3 scripts/plot_boxplot_ablation.py evaluations/ablation.csv
```

#### Selectivity
- MICRO-P
```shell
> python3 scripts/plot_selectivity.py evaluations/micro-p.csv
```

- MICRO-K
```shell
> python3 scripts/plot_selectivity.py evaluations/micro-k.csv
```

#### Spectrum
```shell
> python3 scripts/plot_spectrum.py evaluations/spectrum_q1.csv -t q1a
> python3 scripts/plot_spectrum.py evaluations/spectrum_q2.csv -t q2a
> python3 scripts/plot_spectrum.py evaluations/spectrum_q3.csv -t q3a
> python3 scripts/plot_spectrum.py evaluations/spectrum_q4.csv -t q4a
> python3 scripts/plot_spectrum.py evaluations/spectrum_q5.csv -t q5a
> python3 scripts/plot_spectrum.py evaluations/spectrum_q6.csv -t q6a
```
