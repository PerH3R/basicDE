cmake_minimum_required(VERSION 3.15) 
file(READ VERSION VERSION_STRING)
string(STRIP ${VERSION_STRING} ${VERSION_STRING})
string(REGEX REPLACE "\n$" "" VERSION_STRING "${VERSION_STRING}")
project(ioh    
    VERSION "${VERSION_STRING}"
    LANGUAGES CXX  
) 
add_compile_definitions(PROJECT_VER="${CMAKE_PROJECT_VERSION}")


set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)
add_compile_definitions(FSEXPERIMENTAL)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17 -lstdc++fs") 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb -march=native")
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    link_libraries(stdc++fs) 
    add_compile_definitions(FSEXPERIMENTAL)
endif() 
add_subdirectory(deps/IOHexperimenter)

add_library(simpleDE src/agent.cc  src/adaptation.cc src/population.cc  src/tools.cc src/argparse.cc  src/crossover.cc  src/mutation.cc  src/selection.cc  src/boundary.cc
            include/credit.h)
target_include_directories(simpleDE PUBLIC include deps/IOHExperimenter/include)

target_link_libraries(simpleDE ioh)

add_executable(main main/main.cc)
target_link_libraries(main PUBLIC simpleDE)




