/**
 * \file astar.h
 * \brief A* implementation of Boxed In Level-Solver.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#ifndef ASTAR_H__
#define ASTAR_H__

#include <stdint.h>

#include <list>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "boxedindefs.h"
#include "boxedintypes.h"
#include "BoxedInNode.h"
#include "LevelState.h"
#include "SearchResult.h"


/**
    \mainpage Boxed In Solver

    \tableofcontents

    \section libboxedin_sec libboxedin C++ API

    This library contains the A* search algorithm for Boxed In, as well as types
    and functions common to the search algorithm and utility io functions.

    \ref boxedin\n
    \ref boxedin::io\n

    \section boxedinsolver_sec BoxedInSolver

    This is the command line program for running the solver.

    \ref boxedinsolver_page

    \section viewsolution_sec ViewSolution

    This is the command line tool for viewing a solution.

    \ref viewsolution_page

    \section build_instructions_sec Build Instructions

    Instructions for building on each platform.

    \ref build_instructions_page
 */



/**
    \page boxedinsolver_page BoxedInSolver

    \section description_sec Description
    Command line program for running Boxed In A* search. The solution (if found)
    is written to standard output. All other output is written to standard
    error.

    \section usage_sec Usage

    \verbatim
BoxedInSolver OPTIONS file
    Reads a boxed in level from file and runs the solving
    algorithm for it.

    OPTIONS
        -c
            Print console output in color
        -s, <statsfile>
            Print solver stats to statsfile
    \endverbatim

    \section examples_sec Command Examples

Solve level 1

    \verbatim
bin/BoxedInSolver levels/1/01.txt
    \endverbatim

Solve level 7 and print to console in color

    \verbatim
bin/BoxedInSolver -c levels/1/07.txt
    \endverbatim

Solve level 1, game 2, print to console in color and write solution to a
file by redirecting standard input.

    \verbatim
bin/BoxedInSolver -c levels/2/01.txt > solutions/2/01.txt
    \endverbatim

Same as before, but write algorithm statistics to a file.

    \verbatim
bin/BoxedInSolver -c -s stats/2/01.txt levels/2/01.txt > solutions/2/01.txt
    \endverbatim

    \section example_level_file_sec Example Level File

levels/1/13.txt

    \verbatim
xxxxxxxxxx
x        x
x x+x+xg+x
x x x    x
x +   p  x
x*x*xxx xx
'xxxxxx x'
xr    x xx
'xx      x
'''x  x +x
xxxxx x *x
x@ RG   xx
xxxxxxxxx'
''''''''''
    \endverbatim
 
    \tableofcontents
 */



/**
    \page viewsolution_page ViewSolution

    \section description_sec Description
    Command line program for viewing a Boxed In solution.

    This program uses a ASCII level file and solution file with U,D,L,R directions
    as input.

    The moves are applied to the level in sequence and printed to the console,
    optionally in color.

    \note No validation is performed to ensure that moves are legal.

    \section usage_sec Usage

    \verbatim
ViewSolution OPTIONS level solution
    Reads a boxed in level from <level> and solution from <solution>, applies
    each move and displays the result to the console.

    OPTIONS
        -c
            Print console output in color
        -n NUM
            View solution NUM times before exiting.
	    -1 will loop forever (default).
    \endverbatim

    \section examples_sec Examples

View solution to game 1, level 7

    \verbatim
bin/ViewSolution levels/1/07.txt solutions/1/07.txt
    \endverbatim

Same as above, but print in color

    \verbatim
bin/ViewSolution -c levels/1/07.txt solutions/1/07.txt
    \endverbatim

    \section example_level_file_sec Example Level File

levels/1/07.txt

    \verbatim
''xxxxxxxx
''xg    *x
xxx x x+xx
x    + + x
x x x x xx
x    +  *x
x++xxxxxxx
x  *x'''''
x   xxx'''
x x + xx''
x r ++x@xx
xp  + R Gx
xxxxxxxxxx
''''''''''
    \endverbatim

    \section example_solution_file_sec Example Solution File

solutions/1/07.txt

    \verbatim
UUUUUURRRRUURRDDRLLLLLUUUURRDURRRLDDLLLRDDLLUUUDDDLDDLDRURDDRLULLDDDRRRULDRRRRU
    \endverbatim

    \tableofcontents
 */



/**
   \page build_instructions_page Build Instructions

   \section build_instructions_sec Build Instructions

   Use these build instructions to build the following:

   - bin/BoxedInSolver
   - bin/ViewSolution

   \subsection build_instructions_linux_sec Build Instructions Linux

   -# Open terminal and cd to the top-level Boxed In Solver directory.
   -# make

   \subsection build_instructions_windows_sec Build Instructions Windows

   -# From Visual Studio, open the .sln file under the msvc folder.
   -# Hit F7 to build.

   \subsection see_also_sec See Also

   \ref boxedinsolver_page\n
   \ref viewsolution_page
 */



/**
 * \namespace boxedin
 * \brief Core components of the Boxed In library.
 */
namespace boxedin {


/**
   \struct FValCompare
   \brief This is a functor used to compare BoxedInNode's by f-value
*/
struct FValCompare
{
    bool operator()(const BoxedInNode* l, const BoxedInNode* r) const
    {
        return l->osd->f > r->osd->f;
    };
};



/**
   \struct KeyHash
   \brief This is a functor for returning a BoxedInNode hash key
*/
struct KeyHash
{
    size_t operator()(const BoxedInNode* node) const
    {
        return node->key;
    }
};



/**
   \struct KeyEqual
   \brief This is a functor for comparison by hash key
*/
struct KeyEqual
{
    bool operator()(const BoxedInNode* l, const BoxedInNode* r) const
    {
        return l->key == r->key;
    };
};



typedef std::priority_queue<BoxedInNode*,
                            std::vector<BoxedInNode*>,
                            FValCompare> OpenQueue;

typedef std::unordered_set<BoxedInNode*,
                           KeyHash,
                           KeyEqual> OpenSet;
typedef OpenSet::iterator OpenSetIt;
typedef OpenSet::const_iterator OpenSetCit;

typedef std::unordered_set<BoxedInNode*,
                           KeyHash,
                           KeyEqual> ClosedSet;
typedef ClosedSet::iterator ClosedSetIt;
typedef ClosedSet::const_iterator ClosedSetCit;


SearchResult AStarSearch(const BoxedInNode& start);
BoxedInNodeList GetSuccessors(const BoxedInNode& node);
bool IsGoal(const BoxedInNode& node);
cost_t HeuristicCostEstimate(const BoxedInNode& node);

ActionPoints FindActionPoints(const BoxedInNode& node);
bool IsActionPoint(const charmap& cmap, const Coord& coord);
cost_t ShortestMovesThroughPoints(const Coord& player,
                                  const Gears& gears,
                                  gears_bitfield_t gears_bitfield,
                                  const Coord& exit);
cost_t MovesThroughPoints(const Coords& coords);
bool IsGear(const charmap& cmap, const Coord& coord);
bool IsExit(const charmap& cmap, const Coord& coord);
bool CanBoxMoveUp(const charmap& cmap, const Coord& coord);
bool CanBoxMoveDown(const charmap& cmap, const Coord& coord);
bool CanBoxMoveLeft(const charmap& cmap, const Coord& coord);
bool CanBoxMoveRight(const charmap& cmap, const Coord& coord);


} // namespace


#endif
