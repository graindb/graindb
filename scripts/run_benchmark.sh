#!/bin/bash
./build/release/benchmark/benchmark_runner "IMDB_RAIQ[0-9][0-9][0-9]A" --out=logs/imdb/imdb_ra_join_merge.out --log=logs/imdb/imdb_rai_join_merge.log
#./build/release/benchmark/benchmark_runner "LDBCQ00[012345789]A|LDBCQ01[2-9]A|LDBCQ02[025789]A|LDBCQ03[035678]A|LDBCQ04[1345]A" --out=logs/ldbc/sf1_rai_test.out --log=logs/ldbc/sf1_rai_test.log
#./build/release/benchmark/benchmark_runner "LDBCQ00[12345789]|LDBCQ01[0123456789]|LDBCQ02[25789]|LDBCQ03[023568]|LDBCQ04[1345]" --out=logs/ldbc/sf1_test.out --log=logs/ldbc/sf1_test.log
#./build/release/benchmark/benchmark_runner "LDBCQ00[12345789]A|LDBCQ01[0123456789]A|LDBCQ02[25789]A|LDBCQ03[023568]A|LDBCQ04[1345]A" --out=logs/ldbc/sf1_rai_test.out --log=logs/ldbc/sf1_rai_test.log
