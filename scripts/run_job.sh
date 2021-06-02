#!/bin/bash
make && make debug
./build/release/benchmark/benchmark_runner "IMDB_SMALL_NEW_Q[0-9][0-9][0-9]|IMDB_SMALL_NEW_Q[0-9][0-9][0-9]A" --out=imdb_small_test.out --log=imdb_small_test.log
