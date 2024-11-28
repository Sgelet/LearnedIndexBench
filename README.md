# Testbed for Dynamic Indexing Through Learned Indices with Worst-case Guarantees

This is the testbed used for experimental evaluation as presented in the paper "Dynamic Indexing Through Learned Indices with Worst-case Guarantees".
It contains scripts to generate synthetic data, summary statistics, as well as perform evaluations of performance in dynamic scenarios. Included is also a rudimentary tool to replicate plots as shown in the paper, as well as parse OSM data.

## Build options

The bench contains various compile-time options to control parameters such as the $\epsilon$-value. They are automatically supplied when building the bench using the script, and are as follows:

- `EPSILON=x` sets the value of epsilon to x
- `MODEL={0,1}` determines the used index. Setting `MODEL=0`uses the dynamic PGM, otherwise our index is used
- `OPT={true,false}` setting `OPT=true` uses slanted line segments, whereas `false` uses vertical.
- `NOSTORE={true,false}` controls if the index should also provide a storage structure for queries,

## Run options

To manually run tests, simply run a binary with the following program arguments, and supply the data through standard in:

- `RUN X Y Z` runs the dynamic update scenario with X operations, query ratio Y and insertion ratio (of the remaining operations) Z.
- `ADV X` runs the adversarial dynamic scenario with X query operations following deletions
- `COUNT X` runs linecounting, reporting the number of segments every X insertions.

## Running the testbed

We provide a number of scripts to automate things. Each is located in, and should be executed from, the repository root.
By default the tests run on every `.data` file found in the `data/` folder, saving output to files in the `out/` folder.

- `./build_bench.sh` compiles and builds the various versions of learned indices, storing the resulting binaries in `/bin`.
- `./run.sh` runs the dynamic scenarios as described in the paper.
- `./linecount.sh` runs the summary statistics test.

## Plots

We provide a very rudimentary tool for generating plots. The Python program `output_parser.py` can be used to reproduce our plots by running `cat outputfile | python3 output_parser.py`. The type of plot is determined by the filetype of the outputfile (`.linecount`for linecounts, `.run` for dynamic tests).