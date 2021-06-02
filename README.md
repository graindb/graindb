# GrainDB
![C/C++ CI](https://github.com/graindb/graindb/workflows/C/C++%20CI/badge.svg)

GrainDB is based on DuckDB.

# Requirements
GrainDB requires [CMake](https://cmake.org) to be installed and a `C++11` compliant compiler. GCC 4.9 and newer, Clang 3.9 and newer and VisualStudio 2017 are tested on each revision.

## Compiling
Run `make` in the root directory to compile the sources. For development, use `make debug` to build a non-optimized debug version. You may run `make unit` and `make allunit` to verify that your version works properly after making changes.

# Evaluation
## LDBC
### Prepare a standalone Hadoop cluster
- `cd hadoop-2.9.2`.
- Remove old data. `rm -rf data/current`.
- Reformat. `./bin/hdfs namenode -format`.
- Start the cluster. `./sbin/start-dfs.sh`, `./sbin/start-yarn.sh`.

### Prepare data generation params and Run
- `cd ldbc_snb_datagen`.
- Modify `vim params.ini`: SF, data generation location.
- Run. `./run.sh`
- Copy files to local. `cd hadoop-2.9.2 && ./bin/hadoop fs -copyToLocal /home/g35jin/ldbc_dataset/sf1/social_network /home/g35jin/ldbc_dataset/sf1`.

### Process generated data according to the Postgres fashion
- `cd graindb`
- `./scripts/load_ldbc.sh`

### Generate query params

We also do ldbc_benchmark in DuckDB of the latest version, refer to ```graindb/ldbc_benchmark_duckdb_latest ``` . The result of benchmark can be seen in [excel]( https://docs.google.com/spreadsheets/d/17s3m4Tf3-It_-SrLAidmFnJP-3pBNR_FdkofnTRuxHE/edit?usp=sharing).
