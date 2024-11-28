#!/bin/bash

declare -a epsilons=(
  "16"
  "32"
  "64"
  "128"
  "256"
)

mkdir build
rm build/*
mkdir bin
rm bin/*
cd build

for i in "${epsilons[@]}"
do
  echo "Compiling for epsilon $i"
  cmake -DCMAKE_CXX_FLAGS="-DMODEL=0 -DEPSILON=$i" -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .
  mv LearnedIndexBench ../bin/LearnedIndexBenchPGM$i
  rm -rf ./*
  cmake -DCMAKE_CXX_FLAGS="-DOPT=true -DMODEL=1 -DSTORAGE=false -DEPSILON=$i" -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .
  mv LearnedIndexBench ../bin/LearnedIndexBenchOPT$i
  rm -rf ./*
  cmake -DCMAKE_CXX_FLAGS="-DOPT=false -DMODEL=1 -DEPSILON=$i" -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .
  mv LearnedIndexBench ../bin/LearnedIndexBenchIDX$i
  rm -rf ./*
  cmake -DCMAKE_CXX_FLAGS="-DOPT=false -DMODEL=1 -DSTORAGE=false -DEPSILON=$i" -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .
  mv LearnedIndexBench ../bin/LearnedIndexBenchNOSTORE$i
  rm -rf ./*
done