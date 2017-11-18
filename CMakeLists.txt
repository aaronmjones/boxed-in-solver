cmake_minimum_required (VERSION 3.5)

project(BoxedInSolver)

find_package(Boost COMPONENTS program_options)

set (CMAKE_CXX_STANDARD 11)

add_executable(solve
               src/astar.cc
               src/boxedinio.cc
               src/BoxedInSolver.cc
               src/Heuristic.cc
               src/Level.cc
               src/LevelState.cc
               src/memusage.cc
               src/Node.cc
)

target_include_directories(solve PRIVATE
                           ${CMAKE_SOURCE_DIR}/hdr
                           ${Boost_INCLUDE_DIRS}
)

target_link_libraries(solve PRIVATE
                      ${Boost_LIBRARIES}
)

install(TARGETS solve
        RUNTIME DESTINATION bin
)
