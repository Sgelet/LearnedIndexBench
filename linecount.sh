#!/bin/bash

declare -a epsilons=(
  "64"
)

mapfile -t files <<< "$(ls -1 data/*.data | grep -v _[0-9] | grep -v books | grep -v italy)"

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
    cat "${files[$f]}" | ./bin/LearnedIndexBenchPGM$i COUNT 100000 >> out/${filename}_PGM_$i.linecount
    echo "Linecounting OPT epsilon=$i"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchOPT$i COUNT 100000 >> out/${filename}_OPT_$i.linecount
    #echo "Linecounting IDX epsilon=$i"
    #cat "${files[$f]}" | ./bin/LearnedIndexBenchIDX$i COUNT 100000 >> out/${filename}_IDX_$i.linecount
    echo "Linecounting NOSTORE epsilon=$i"
    cat "${files[$f]}" | ./bin/LearnedIndexBenchNOSTORE$i COUNT 100000 >> out/${filename}_NOSTORE_$i.linecount
  done
done