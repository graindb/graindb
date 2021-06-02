#!/bin/bash
./build/release/benchmark/benchmark_runner "Q02|Q08|Q09|Q11|Q14|Q16|Q17|Q19|Q20|Q21" --out=../GRBench/tpch/logs/sf1/duck.out --log=../GRBench/tpch/logs/sf1/duck.log
./build/release/benchmark/benchmark_runner "SF1_RAIQ0[289]|SF1_RAIQ1[14679]|SF1_RAIQ2[01]" --out=../GRBench/tpch/logs/sf1/grain.out --log=../GRBench/tpch/logs/sf1/grain.log
./build/release/benchmark/benchmark_runner "SF10_Q0[289]|SF10_Q1[14679]|SF10_Q2[01]" --out=../GRBench/tpch/logs/sf10/duck.out --log=../GRBench/tpch/logs/sf10/duck.log
./build/release/benchmark/benchmark_runner "SF10_RAIQ0[289]|SF10_RAIQ1[14679]|SF10_RAIQ2[01]" --out=../GRBench/tpch/logs/sf10/grain.out --log=../GRBench/tpch/logs/sf10/grain.log
