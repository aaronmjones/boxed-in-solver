#!/bin/bash
#
# Filename: solve-and-validate.sh
# Description:
# Solve level(s) using solver and validate solution using validate.
################################################################################

set -eo pipefail

function usage() {
echo "
SYNOPSIS
  solve-and-validate.sh <game-number:level-number> ... 

DESCRIPTION
  This script uses the BoxedInSolver to attempt to solve each level specified
  by game-number and level-number and on success uses ViewSolution to animate
  the solution move-by-move.

EXAMPLES
  Solve and view game 1, level 7:
  solve-and-validate.sh 1:7

AUTHOR
  Aaron Jones    ajones2279@gmail.com    30 Dec 2014
" >&2
}

# TODO: rename view-solution program to validate-solution and use it
#       for automated testing. Add a flag to view the animated solution.
#       Print UDLR movements while animating solution. Throw exception
#       if solution violates game physics.
for GAMELEVEL; do
    IFS=':' read GAME LEVEL <<< "$GAMELEVEL"
    LEVEL_FILE=$(printf %02d.txt $LEVEL)
    SOLUTION_FILE=$(printf %02d.txt $LEVEL)
    STATS_FILE=$(printf %02d.stats $LEVEL)
    ./solve -l level-data/$GAME/$LEVEL_FILE -s solution-data/$GAME/$STATS_FILE > solution-data/$GAME/$SOLUTION_FILE
    ./validate -l level-data/$GAME/$LEVEL_FILE -s solution-data/$GAME/$SOLUTION_FILE
    echo -e "\n\n"
done
