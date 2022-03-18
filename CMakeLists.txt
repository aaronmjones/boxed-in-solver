cmake_minimum_required (VERSION 3.11)

project(BoxedIn)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(fmt)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# solve -----------------------------------------------------------------------

add_executable(solve
               src/solve.cc
               src/astar.cc
               src/boxedinio.cc
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
                      fmt::fmt
)

# validate --------------------------------------------------------------------

add_executable(validate
               src/validate.cc
               src/boxedinio.cc
               src/Level.cc
)

target_include_directories(validate PRIVATE
                           ${Boost_INCLUDE_DIRS}
)

target_link_libraries(validate PRIVATE
                      ${Boost_LIBRARIES}
                      fmt::fmt
)

install(TARGETS solve validate
        RUNTIME DESTINATION bin
)

# files ------------------------------------------------------------------------

file(COPY scripts DESTINATION .)
file(COPY level-data DESTINATION .)
file(COPY solution-data DESTINATION .)

# tests ------------------------------------------------------------------------

add_subdirectory(test)
