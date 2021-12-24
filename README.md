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

#### Draw the boxplot
```shell
>
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

#### Draw the boxplot
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

#### Run GraphflowDB on SNB-M:
```shell
>
```

## Detailed Evaluation
### Ablation Study (8.3.1)

### Performance of Predefined Joins Under Varying Entity vs Relationship Table Selectivity (8.3.2)

### Plan Spectrum Analyses (8.3.3)

### Storage Costs (8.3.4)
