#!/bin/bash

declare -a epsilons=(
  #"16"
  #"32"
  "64"
  #"128"
  #"256"
)

declare -a ratios=(
  "0.0"
  "0.25"
  "0.5"
  "0.75"
  "1.0"
)

declare -a del_ratios=(
  "0.0"
  "0.25"
  "0.5"
  "0.75"
)

mapfile -t files <<< "$(ls -1 data/*.data)"

mkdir out
rm -f out/*.run

for f in "${!files[@]}";
do
  full="${files[$f]##*/}"
  filename="${full%.*}"
  echo "${files[$f]}"
  for i in "${epsilons[@]}"
  do
    for r in "${ratios[@]}"
    do
      for d in "${del_ratios[@]}"
      do
        echo "Running PGM epsilon=$i, ratio=$r, del_ratio=$d"
        cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i RUN 10000000 $r 0.5 $d >> out/${filename}_pgm_$i_$r_$d.run
        echo "Running IDX epsilon=$i, ratio=$r, del_ratio=$d"
        cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i RUN 10000000 $r 0.5 $d >> out/${filename}_idx_$i_$r_$d.run
      done
    done
    echo "Running PGM epsilon=$i, adversarial"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i ADV 10000000 >> out/${filename}_pgm_$i.run_adv
    echo "Running IDX epsilon=$i, adversarial"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i ADV 10000000 >> out/${filename}_idx_$i.run_adv
  done
done