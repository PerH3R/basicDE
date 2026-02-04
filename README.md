# basicDE
A not-so-basic-anymore customizable Differential Evolution algorithm using a Multi-Armed Bandit approach for Adaptive Operator Selection.  
[IOHprofiler](https://iohprofiler.github.io/) is used to generate problems and post-DE analysis, IOHprofiler also allows for [custom](https://github.com/IOHprofiler/IOHexperimenter/tree/master/example#add-new-problems) problems.  
A detailed explanation of command line arguments can be found in the [argparse header](include/argparse.h).

New operators can be easily added, already included are:
- 16 mutation operators: variations of [rand|best|target]-to-[rand|best|target]/[1|2] and some more novel ideas I found here and there.
- 2 crossover operators: Binomial and Exponential.
- 3 Boundary operators: Clamp, Reflect, Reinitialize. Also allows for resampling (reattempting the mutation step when infeasable vectors are generated).
- 4 credit assignment operators: fitness, tanh(fitness), Binary improvement, Binary+ improvement.
- 1 selection operator: Elitist.


Adding new operators: 
1. Code the implementation into the respective .h and .cc file (you can use an existing operator as template).
2. Add an entry to the respective [operators.h](include/operators.h) enums.
3. Add a case to the relevant 'create_[type]_operator()' funtion in [population.cc](include/population.cc)

### Building the project: 
Have `/IOHexperimenter/` and `/BasicDE/` in the same folder. \
From `/BasicDE/ ` :
```
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=[Debug|Release]  && sudo make install -j [core_count]
```

### Thesis
This program was made for a thesis project. The accompanying thesis can be found at the [LIACS Thesis Repository](https://theses.liacs.nl/3270)
