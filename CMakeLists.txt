cmake_minimum_required (VERSION 3.15)
project (simpleDE
		VERSION "0.3.9"
)
set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)
add_compile_definitions(FSEXPERIMENTAL)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17 -lstdc++fs") 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    link_libraries(stdc++fs) 
    add_compile_definitions(FSEXPERIMENTAL)
endif() 
add_subdirectory(deps/IOHexperimenter)


add_library(simpleDE src/agent.cc  src/adaptation.cc src/population.cc  src/tools.cc src/argparse.cc  src/crossover.cc  src/mutation.cc  src/selection.cc  src/boundary.cc)
target_include_directories(simpleDE PUBLIC include deps/IOHExperimenter/include)

target_link_libraries(simpleDE ioh)

add_executable(main main/main.cc)
target_link_libraries(main PUBLIC simpleDE)




