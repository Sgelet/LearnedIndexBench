#!/bin/bash


mkdir data
#rm -f data/*

#echo "Generating line data"
#./bin/LearnedIndexBenchPGM16 GEN LINES 1000000 5 4 | shuf > data/lines_6_5_4.data
echo "Generating random data"
./bin/LearnedIndexBenchPGM16 GEN RAN 50000000 100000000000 | shuf > data/ran_75_11.data
echo "Generation done"