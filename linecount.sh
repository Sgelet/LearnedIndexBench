#!/bin/bash

declare -a epsilons=(
  "64"
)

mapfile -t files <<< "$(ls -1 data/sec*.data)"

mkdir out
rm -f out/*.linecount

for f in "${!files[@]}";
do
  full="${files[$f]##*/}"
  filename="${full%.*}"
  echo "${files[$f]}"
  for i in "${epsilons[@]}"
  do
    echo "Linecounting PGM epsilon=$i"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i COUNT 50000 >> out/${filename}PGM_$i.linecount
    echo "Linecounting OPT epsilon=$i"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchOPT$i COUNT 50000 >> out/${filename}OPT_$i.linecount
    #echo "Linecounting IDX epsilon=$i"
    #cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i COUNT 50000 >> out/${filename}IDX_$i.linecount
    #echo "Linecounting NOSTORE epsilon=$i"
    #cat "${files[$f]}" | ./bin/LearnedIndexBenchNOSTORE$i COUNT 50000 >> out/${filename}NOSTORE_$i.linecount
  done
done