# basicDE
A not-so-basic-anymore higly-customizable Differential Evolution algorithm using a Multi-Armed Bandit approach for Adaptive Operator Selection.

[IOHprofiler](https://iohprofiler.github.io/) is used to generate problems and post-DE analysis, IOHprofiler also allows for [custom](https://github.com/IOHprofiler/IOHexperimenter/tree/master/example#add-new-problems) problems.

New operators can be easily added, but included are:
- 16 mutation operators: variations of [rand|best|target]-to-[rand|best|target]/[1|2] and a few more 'exotic' operators.
- 2 crossover operators: Binomial and Exponential.
- 3 Boundary operators: Clamp, Reflect, Reinitialize. Also allows for resampling (reattempting the mutation step when infeasable vectors are generated).
- 4 credit assignment operators: fitness, tanh(fitness), Binary improvement, Binary+ improvement.
- 1 selection operator: Elitist.

To add new operators, code them into the respective .h and .cc file and add an entry to the [operators.h](include/operators.h) file to make it selectable.

A detailed explanation of command line flags can be found in the [argparse header](include/argparse.h).

### To make: 
Have `/IOHexperimenter/` and `/BasicDE/` in the same folder. \
From `/BasicDE/ ` :
```
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=[Debug|Release]  && sudo make install -j [core_count]
```
