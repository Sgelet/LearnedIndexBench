#!/bin/bash

declare -a epsilons=(
  "64"
)


mapfile -t files <<< "$(ls -1 data/sec*.data)"

mkdir out
rm -f out/*.run_udh

for f in "${!files[@]}";
do
  full="${files[$f]##*/}"
  filename="${full%.*}"
  echo "${files[$f]}"
  for i in "${epsilons[@]}"
  do
    echo "Running PGM epsilon=$i, delete heavy 0.1 "
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i RUN 5000000 0.2 0.1 >> out/${filename}_pgm_del_$i.run_del
    echo "Running IDX epsilon=$i, delete heavy 0.1"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i RUN 5000000 0.2 0.1 >> out/${filename}_idx_del_$i.run_del
    echo "Running PGM epsilon=$i, delete heavy 0.25 "
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i RUN 12500000 0.2 0.1 >> out/${filename}_pgm_del_$i.run_del
    echo "Running IDX epsilon=$i, delete heavy 0.25"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i RUN 12500000 0.2 0.1 >> out/${filename}_idx_del_$i.run_del
    echo "Running PGM epsilon=$i, delete heavy 0.5"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i RUN 25000000 0.2 0.1 >> out/${filename}_pgm_del_$i.run_del
    echo "Running IDX epsilon=$i, delete heavy 0.5"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i RUN 25000000 0.2 0.1 >> out/${filename}_idx_del_$i.run_del
    echo "Running PGM epsilon=$i, adversarial"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i ADV 10000000 >> out/${filename}_pgm_adv_$i.run_adv
    echo "Running IDX epsilon=$i, adversarial"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i ADV 10000000 >> out/${filename}_idx_adv_$i.run_adv
  done
done