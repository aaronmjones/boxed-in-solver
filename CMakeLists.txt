cmake_minimum_required (VERSION 3.11)

project(BoxedInSolver)

find_package(Boost REQUIRED COMPONENTS program_options)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# solve ------------------------------------------------------------------------

add_executable(solve
               src/astar.cc
               src/boxedinio.cc
               src/BoxedInSolver.cc
               src/Heuristic.cc
               src/Level.cc
               src/memusage.cc
               src/Node.cc
)

target_include_directories(solve PRIVATE
                           ${Boost_INCLUDE_DIRS}
)

target_link_libraries(solve PRIVATE
                      ${Boost_LIBRARIES}
)

# view-solution ----------------------------------------------------------------

add_executable(view-solution
               src/boxedinio.cc
               src/Level.cc
               src/ViewSolution.cc
)

target_include_directories(view-solution PRIVATE
                           ${Boost_INCLUDE_DIRS}
)

target_link_libraries(view-solution PRIVATE
                      ${Boost_LIBRARIES}
)

install(TARGETS solve view-solution
        RUNTIME DESTINATION bin
)

# tests ------------------------------------------------------------------------

add_subdirectory(test)
