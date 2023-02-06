cmake_minimum_required (VERSION 3.1)
project (simpleDE)
set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)
add_compile_definitions(FSEXPERIMENTAL)
set(CXX_FLAGS  "${CXX_FLAGS} -std=c++17 -g -Wall -Wextra -fopenmp -Wreturn-type -lstdc++fs")
add_subdirectory(deps/IOHexperimenter)


add_library(simpleDE src/agent.cc      src/functions.cc  src/population.cc  src/tools.cc
src/crossover.cc  src/mutation.cc   src/selection.cc	src/boundary.cc)
target_include_directories(simpleDE PUBLIC include deps/IOHExperimenter/include)

target_link_libraries(simpleDE ioh)

add_executable(main main/main.cc)
target_link_libraries(main PUBLIC simpleDE)




