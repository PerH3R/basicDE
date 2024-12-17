# basicDE
A not-so-basic anymore customisable Differential Evolution algorithm using a Multi-Armed Bandit approach for Adaptive Operator Selection.

It uses IOHprofiler (https://iohprofiler.github.io/) for problems and analysis, IOHprofiler also allows for custom problems.

Includes 16 mutation operators, 2 crossover operators, 3 Boundary operators + resampling, 4 credit assignment operators, 1 selection operator and allows for easy addition of new operators. To add new operators, add them to the respective .h and .cc file and to the operators.h file.

A detailed explanation of command line flags can be found in the [argparse header](include/argparse.h).

To make:
Have `/IOHexperimenter/` and `/BasicDE/` in the same folder. \
From `/BasicDE/ ` :
```
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=[Debug|Release]  && sudo make install -j [Core_count]
```
