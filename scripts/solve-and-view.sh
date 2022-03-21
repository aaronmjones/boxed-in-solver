#!/bin/bash
#
# Filename: solve_and_view.sh
# Description:
# Solve level(s) using BoxedInSolver and view animated solution using
# ViewSolution.
################################################################################

function usage() {
echo "
SYNOPSIS
  solve_and_view.sh <game-number:level-number> ... 

DESCRIPTION
  This script uses the BoxedInSolver to attempt to solve each level specified
  by game-number and level-number and on success uses ViewSolution to animate
  the solution move-by-move.

EXAMPLES
  Solve and view game 1, level 7:
  solve_and_view.sh 1:7

AUTHOR
  Aaron Jones    ajones2279@gmail.com    30 Dec 2014
" >&2
}

for GAME in {1..3}; do
    if [ ! -d "stats/$GAME" ]; then
        mkdir -p "stats/$GAME"
    fi
    if [ ! -d "solutions/$GAME" ]; then
        mkdir -p "solutions/$GAME"
    fi
done

# TODO: rename view-solution program to validate-solution and use it
#       for automated testing. Add a flag to view the animated solution.
#       Print UDLR movements while animating solution. Throw exception
#       if solution violates game physics.
for GAMELEVEL; do
    IFS=':' read GAME LEVEL <<< "$GAMELEVEL"
    LEVEL_FILE=$(printf %02d.txt $LEVEL)
    SOLUTION_FILE=$(printf %02d.txt $LEVEL)
    STATS_FILE=$(printf %02d.stats $LEVEL)
    if ./solve -l level-data/$GAME/$LEVEL_FILE -s solution-data/$GAME/$STATS_FILE > solution-data/$GAME/$SOLUTION_FILE; then
        ./view-solution -l level-data/$GAME/$LEVEL_FILE -s solution-data/$GAME/$SOLUTION_FILE
    fi
done
