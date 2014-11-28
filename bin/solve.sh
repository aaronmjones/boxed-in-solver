#!/bin/bash
#
# USAGE:
# solve.sh OPTIONS gamenumber levelnumber [levelnumber]
#
# OPTIONS
#   -p, --post Post results to solverstats.com using the following
#              environment variables:
#              TOKEN (required)
#              GAMENUMBER (required)
#              LEVELNUMBER (required)
#              META1 (optional)
#              META2 (optional)
#              META3 (optional)
#              COMMENT (optional)
#              
# DESCRIPTION:
# Solve level(s) in the range levelnumber[..levelnumber] for Boxed In
# gamenumber.
############################################################

SOLVERSTATSURL="http://www.solverstats.com/Solver/SubmitResult"

while getopts "p" opt; do
  case $opt in
    p)
      POST=true
      if [ x$TOKEN = x ]; then
        echo "Error: In order to post results, environment variable TOKEN must be set." >&2
	echo "Note: META1, META2 and META3 are optional." >&2
	exit 1
      fi
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

shift $((OPTIND-1))

GAMENUMBER=$1
LEVEL=$2
LEVEL_LAST=${3:-$2}

if [ ! -d "stats/$GAMENUMBER" ]; then
    mkdir -p "stats/$GAMENUMBER"
fi
if [ ! -d "solutions/$GAMENUMBER" ]; then
    mkdir -p "solutions/$GAMENUMBER"
fi

for LEVELNUMBER in $(seq $LEVEL $LEVEL_LAST)
do
    file=$(printf %02d.txt $LEVELNUMBER)
    bin/BoxedInSolver levels/$GAMENUMBER/$file -s stats/$GAMENUMBER/$file > solutions/$GAMENUMBER/$file

    if [ $? == 0 ] && [ x$POST != x ]; then
	echo "Submitting results to $SOLVERSTATSURL"
        SOLUTION=$(cat solutions/$GAMENUMBER/$file | tr -d '[:space:]' | tr '[:lower:]' '[:upper:]')
	SOLVETIME=$(grep SOLVETIME stats/$GAMENUMBER/$file | awk '{print $2}')
	COMMENT=$(tail -n +2 stats/$GAMENUMBER/$file | tr '\n' ' ')
	meta1=${META1:+"&meta1=$META1"}
	meta2=${META2:+"&meta2=$META2"}
	meta3=${META3:+"&meta3=$META3"}
        curl --data "token=$TOKEN&gameNumber=$GAMENUMBER&levelNumber=$LEVELNUMBER&solution=$SOLUTION&solveTime=$SOLVETIME&comment=$COMMENT$meta1$meta2$meta3" $SOLVERSTATSURL
    fi
done
