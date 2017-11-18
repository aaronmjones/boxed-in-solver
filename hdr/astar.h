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
#include "LevelState.h"
#include "SearchResult.h"
#include "Node.h"
#include "Level.h"
#include "Heuristic.h"

/**
 * \namespace boxedin
 * \brief Core components of the Boxed In library.
 */
namespace boxedin {

SearchResult astar(Level& level, Heuristic& heuristic);

} // namespace


#endif
