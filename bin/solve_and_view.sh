#!/bin/bash
#
# USAGE:
# solve_and_view.sh GAME LEVEL [LEVEL_LAST]
#
# DESCRIPTION:
# Solve levels in the range LEVEL..LEVEL_LAST for Boxed In
# game number GAME. After each level is solved, print the
# solution (ctrl+c to terminate solution view).
############################################################

GAME=$1
LEVEL=$2
LEVEL_LAST=${3:-$2}

if [ ! -d "stats/$GAME" ]; then
    mkdir -p "stats/$GAME"
fi
if [ ! -d "solutions/$GAME" ]; then
    mkdir -p "solutions/$GAME"
fi

for i in $(seq $LEVEL $LEVEL_LAST)
do
    file=$(printf %02d.txt $i)
    if bin/BoxedInSolver levels/$GAME/$file -s stats/$GAME/$file > solutions/$GAME/$file; then
	bin/ViewSolution levels/$GAME/$file solutions/$GAME/$file
    fi
done
