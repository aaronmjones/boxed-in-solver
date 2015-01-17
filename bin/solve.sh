#!/bin/bash
#
# Filename: solve.sh
# Description:
#     Solve Boxed In level(s) and post results to solverstats.com
################################################################################

SOLVERSTATSURL="http://www.solverstats.com/Solver/SubmitResult"

################################################################################
# Functions
################################################################################

function usage() {
echo "
SYNOPSIS
  solve.sh OPTIONS <game-number:level-number> ... 

DESCRIPTION
  This script uses the BoxedInSolver to attempt to solve each level specified
  by game-number and level-number and optionally will post results to
  solverstats.com.

  Note that in order to post results to solverstats.com, some environment
  variables must be set (below).

OPTIONS
  -p, --post Post results to solverstats.com using the following
             environment variables:
             TOKEN (required)
             META1 (optional)
             META2 (optional)
             META3 (optional)

EXAMPLES
  Solve game 1, level 7:
  solve.sh 1:7

  Solve levels 1-50 for games 1-3:
  solve.sh {1..3}:{1..50}

AUTHOR
  Aaron Jones    ajones2279@gmail.com    30 Dec 2014
" >&2
}

while getopts "ph" opt; do
    case $opt in
    p)
	POST=true
	if [ x$TOKEN = x ]; then
	    echo "
Error: In order to post results, environment variable TOKEN must be set.
Note: META1, META2 and META3 are optional." >&2
	    exit 1
	else
	    echo "
If solver succeeds, results will be posted to solverstats.com with:
    TOKEN=$TOKEN
    META1=$META1
    META2=$META2
    META3=$META3" >&2
	    read -p "Proceed? [Y/n] " -n 1 -r ANSWER
	    ANSWER=${ANSWER:-"Y"}
	    if [[ ! $ANSWER =~ ^[Yy]$ ]]; then
		exit 1
	    fi
	fi
	;;
    h)
	usage
	exit 1
	;;
    \?)
	echo "Invalid option: -$OPTARG" >&2
	exit 1
	;;
    esac
done

shift $((OPTIND-1))

for GAME in {1..3}; do
    if [ ! -d "stats/$GAME" ]; then
	mkdir -p "stats/$GAME"
    fi
    if [ ! -d "solutions/$GAME" ]; then
	mkdir -p "solutions/$GAME"
    fi
done

for GAMELEVEL; do
    IFS=':' read GAME LEVEL <<< "$GAMELEVEL"
    FILE=$(printf %02d.txt $LEVEL)
    bin/BoxedInSolver -c levels/$GAME/$FILE -s stats/$GAME/$FILE > solutions/$GAME/$FILE

    if [ $? == 0 ] && [ x$POST != x ]; then
	echo "Submitting results to $SOLVERSTATSURL"
        SOLUTION=$(cat solutions/$GAME/$FILE | tr -d '[:space:]' | tr '[:lower:]' '[:upper:]')
	SOLVETIME=$(grep SEARCHTIME stats/$GAME/$FILE | awk '{print $2}')
	COMMENT=$(tail -n +2 stats/$GAME/$FILE | tr '\n' ' ')
	meta1=${META1:+"&meta1=$META1"}
	meta2=${META2:+"&meta2=$META2"}
	meta3=${META3:+"&meta3=$META3"}
        curl --data "token=$TOKEN&gameNumber=$GAME&levelNumber=$LEVEL&solution=$SOLUTION&solveTime=$SOLVETIME&comment=$COMMENT$meta1$meta2$meta3" $SOLVERSTATSURL
    fi
done
