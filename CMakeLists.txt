cmake_minimum_required(VERSION 3.20)
project(peaZad1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

#add_executable(${PROJECT_NAME} src/main.cpp)
#add_subdirectory(src/graphRepresentation)
#target_include_directories(${PROJECT_NAME} PUBLIC src/graphRepresentation)
#target_link_directories(${PROJECT_NAME} PUBLIC src/graphRepresentation)
#target_link_libraries(${PROJECT_NAME} PUBLIC matrix)
#
add_executable(PEA_ZAD1 src/main.cpp
        src/graphRepresentation/Graph.cpp src/graphRepresentation/Graph.h
        src/graphRepresentation/matrix/IncidenceMatrix.cpp src/graphRepresentation/matrix/IncidenceMatrix.h
        src/graphRepresentation/matrix/IncidenceMatrixDirectedGraph.cpp src/graphRepresentation/matrix/IncidenceMatrixDirectedGraph.h
        src/graphRepresentation/matrix/AdjacencyMatrix.cpp src/graphRepresentation/matrix/AdjacencyMatrix.h
        src/structures/arrayList/ArrayList.tpp src/structures/arrayList/ArrayList.h
        src/structures/arrayList/Edge.cpp src/structures/arrayList/Edge.h
        src/structures/arrayList/AbstractArray.tpp src/structures/arrayList/AbstractArray.h
        src/travellingSalesmanProblem/ExhaustiveSearch.cpp src/travellingSalesmanProblem/ExhaustiveSearch.h
        )
