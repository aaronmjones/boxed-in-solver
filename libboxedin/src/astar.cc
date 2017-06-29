/**
 * \file astar.cc
 * \brief A* implementation of Boxed In Level-Solver.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#include "astar.h"

#include "Node.h"
#include "Level.h"
#include "Heuristic.h"

#include <iostream>
#include <boxedinio.h>

using namespace std;

namespace boxedin {


OpenQueue openqueue;
OpenSet openset;
ClosedSet closedset;


/**
   \brief Search for a solution to the Boxed In level characterized by the
          given BoxedInNode using the A* search algorithm.
   \details The algorithm works by first expanding a node to generate all of
            it's successor nodes -- nodes that can be reached by performing some
            action, e.g. picking up a gear, moving a box or moving to the exit.
            Once a node has been expanded, it is moved to the closed set and all
            of it's successors (that are not already in the closed set or open
            set) are moved to the open set. Then the algorithm continues to 
            choose the highest priority node from the open set until that node
            is the goal node (no gears are left and the player is on the exit
            coordinate) or until the open set is empty (failure).

            Maintaining an open set allows the algorithm to prioritize nodes
            that have not been expanded.

            Maintaining a closed set allows the algorithm to know which nodes
            it has already expanded and avoid expanding them again.

            The nodes in the open set are prioritized by lowest f(x) for a
            given node x, where:

            f(x) = g(x) + h(x)

            g(x) is the move count from start node to node x
            h(x) is a heuristic cost estimate from x to the goal

            By the laws of A* search, h(x) must not over-estimate the cost from
            the x to the goal. This algorithm uses the shortest Manhattan 
            distance from the player position, through all of the existing
            gears (if any), to the exit.

            \note Another important property of a heuristic is that it is
            consistent (monotone). Because of the dynamic nature of the Boxed
            In search space (moving boxes and gates), the above heuristic is
            not consistent. However as stated here
            http://en.wikipedia.org/wiki/Consistent_heuristic a heurstic can
            be made consistent by the following adjustment:

                h'(P) = max(h(P), h(N) - c(N,P))
            where
                N is any node in the graph,
                P is any descendant of N, and
                c(N,P) is the cost of reaching node P from N

    \returns A result containing a success flag, algorithm statistics and
             the optimal solution path if the algorithm succeeded.
 */
SearchResult AStarSearch(const BoxedInNode& start)
{
    // c'tor sets start time. end time is set in succeeded or failed fn.
    SearchResult result;
    BoxedInNodeListIt nodeIt;
    BoxedInNode* node = new BoxedInNode( start );

    openqueue.push( node );
    openset.insert( node );

    while (!openqueue.empty())
    {
        node = openqueue.top();

        if (node->osd->discard)
        {
            openqueue.pop();
            delete node;
            continue;
        }

        if ( IsGoal(*node) )
        {
            result.SetSucceeded(node, openset.size(), closedset.size());
            return result;
        }

        openqueue.pop();
        openset.erase( node );
        closedset.insert( node );

        BoxedInNodeList successors = GetSuccessors( *node );

        for (nodeIt=successors.begin(); nodeIt!=successors.end(); ++nodeIt)
        {
            BoxedInNode* successor = *nodeIt;

            // successor already in closedset?
            ClosedSetIt closedIt = closedset.find(successor);
            if (closedIt != closedset.end())
            {
                delete successor;
                continue;
            }

            // successor already in openset?
            OpenSetIt openIt = openset.find(successor);
            bool is_successor_in_openset = openIt != openset.end();
            bool new_best_g_found = false;

            if (is_successor_in_openset)
            {
                BoxedInNode* opensetnode = *openIt;
                if (successor->osd->g < opensetnode->osd->g)
                {
                    // Flag for discard in openqueue and erase from openset
                    new_best_g_found = true;
                    opensetnode->osd->discard = 1;
                    openset.erase(openIt);
                }
            }

            if (!is_successor_in_openset || new_best_g_found)
            {
                // This is the adjustment made to guarantee a "consistent"
                // heuristic.
                // See http://en.wikipedia.org/wiki/Consistent_heuristic
		// \fixme: Maybe just move this into the heuristic function.
                int hN = node->osd->f - node->osd->g;
                int hP = HeuristicCostEstimate(*successor);
                int hPrime = max<int>(hN - (int)successor->path.size(), hP);

                successor->osd->f = successor->osd->g + hPrime;

                openqueue.push(successor);
                openset.insert(successor);
            }
            else
            {
                delete successor;
            }
        }

        if (node->osd)
        {
            delete node->osd;
            node->osd = NULL;
        }
    }

    result.SetFailed(openset.size(), closedset.size());

    return result;
}



BoxedInNodeList GetSuccessors(const BoxedInNode& node)
{
    BoxedInNodeList successors;
    BoxedInNode* successor;
    ActionPointIt it;

    ActionPoints action_points = FindActionPoints( node );

    charmap lvlmap;
    bool draw_player = false;
    LevelState::MakeLevelMap(lvlmap, node.osd->lvlState, draw_player);

    for (it=action_points.begin(); it!=action_points.end(); ++it)
    {
        const Coord& coord = it->coord;

        // GEAR SUCCESSOR
        if ( IsGear(lvlmap, coord) )
        {
            successor = new BoxedInNode( node );
            successor->ActionPointGear( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
            continue;
        }
        // EXIT SUCCESSOR
        if ( IsExit(lvlmap, coord) && node.osd->lvlState.gears_bitfield == 0 )
        {
            successor = new BoxedInNode( node );
            successor->ActionPointExit( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
            continue;
        }
        // BOX UP SUCCESSOR
        if ( CanBoxMoveUp(lvlmap, coord) )
        {           
            successor = new BoxedInNode( node );
            successor->ActionPointUp( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
        }
        // BOX DOWN SUCCESSOR
        if ( CanBoxMoveDown(lvlmap, coord) )
        {           
            successor = new BoxedInNode( node );
            successor->ActionPointDown( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
        }
        // BOX LEFT SUCCESSOR
        if ( CanBoxMoveLeft(lvlmap, coord) )
        {           
            successor = new BoxedInNode( node );
            successor->ActionPointLeft( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
        }
        // BOX RIGHT SUCCESSOR
        if ( CanBoxMoveRight(lvlmap, coord) )
        {           
            successor = new BoxedInNode( node );
            successor->ActionPointRight( *it );
            successor->osd->g += (cost_t)successor->path.size();
            successor->parent = &node;
            successors.push_back( successor );
        }
    }
    return successors;
}



/**
   \brief Check if node is the goal
   \returns true if node has no more gears and player location is at the exit.
 */
bool IsGoal(const BoxedInNode& node)
{
    return node.osd->lvlState.gears_bitfield == 0 &&
           node.osd->lvlState.player == node.osd->lvlState.exit;
}



/**
   \brief Calculate the shortest Manhattan distance from player through all
          remaining gears to the exit.
   \param[in] node Estimate cost from this node to the goal.
   \returns The estimated number of moves to the goal node.
 */
cost_t HeuristicCostEstimate( const BoxedInNode& node )
{
    const Coord& player = node.osd->lvlState.player;
    const Coord& exit = node.osd->lvlState.exit;
    const Gears& gears = node.osd->lvlState.gears;
    const gears_bitfield_t& gears_bitfield = node.osd->lvlState.gears_bitfield;
    return ShortestMovesThroughPoints( player, gears, gears_bitfield, exit );
}



/**
   \brief Find all action points reachable from node.
   \details An action point is the coordinate where something interesting can
            happen -- step on a gear, move a box or step on the exit.
            Note that a single action point can be used to expand many successor
	    nodes, like when there is a box above and below the action point --
	    the "up" move would generate a successor and the "down" move would
	    generate a successor.
   \returns A list of ActionPoint objects suitable for generating all of node's
            successor nodes.
 */
ActionPoints FindActionPoints(const BoxedInNode& node)
{
    ActionPoints action_points;
    charmap lvlmap;

    // Create char[][] representing level
    bool drawPlayer = false;
    LevelState::MakeLevelMap(lvlmap, node.osd->lvlState, drawPlayer);

    //// Flood fill to find all action points ////

    // Queue of spaces to fill is initially the current player position
    FloodFillQueue flood_fill_queue;
    flood_fill_queue.push( FloodFillNode(node.osd->lvlState.player) );

    while (!flood_fill_queue.empty())
    {
        FloodFillNode ffnode = flood_fill_queue.front();
        Coord& coord = ffnode.coord;

        // Skip node if it's position has been filled
        if (lvlmap[coord.y][coord.x] == FILLED)
        {
            flood_fill_queue.pop();
            continue;
        }

        // If it's an action point, add it to the results
        if (IsActionPoint(lvlmap, coord))
        {
            action_points.push_back( ActionPoint(ffnode.path, ffnode.coord) );
        }

        // Mark current point as filled
        lvlmap[coord.y][coord.x] = FILLED;

        //// Add neighbor nodes in the 4 directions ////

        // UP
        if ( (coord.y > 0) && CAN_FLOOD_FILL(lvlmap[coord.y-1][coord.x]) )
        {
            FloodFillNode up( ffnode ); // copy of node
            up.path.push_back('U');
            up.coord.y--;
            flood_fill_queue.push( up );
        }
        // DOWN
        if ( (coord.y < HEIGHT-1) && CAN_FLOOD_FILL(lvlmap[coord.y+1][coord.x]) )
        {
            FloodFillNode down( ffnode ); // copy of node
            down.path.push_back('D');
            down.coord.y++;
            flood_fill_queue.push( down );
        }
        // LEFT
        if ( (coord.x > 0) && CAN_FLOOD_FILL(lvlmap[coord.y][coord.x-1]) )
        {
            FloodFillNode left( ffnode ); // copy of node
            left.path.push_back('L');
            left.coord.x--;
            flood_fill_queue.push( left );
        }
        // RIGHT
        if ( (coord.x < WIDTH-1) && CAN_FLOOD_FILL(lvlmap[coord.y][coord.x+1]) )
        {
            FloodFillNode right( ffnode ); // copy of node
            right.path.push_back('R');
            right.coord.x++;
            flood_fill_queue.push( right );
        }
    }

    return action_points;
}



/**
   \brief Check if coordinate in character map is an action point.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
   \returns true if coord is an action point.
 */
bool IsActionPoint(const charmap& cmap, const Coord& coord)
{
    char c = cmap[coord.y][coord.x];

    if (c == EXIT)
    {
        return true;
    }
    if (c == GEAR)
    {
        return true;
    }
    if ( CanBoxMoveUp(cmap, coord) )
    {
        return true;
    }
    if ( CanBoxMoveDown(cmap, coord) )
    {
        return true;
    }
    if ( CanBoxMoveLeft(cmap, coord) )
    {
        return true;
    }
    if ( CanBoxMoveRight(cmap, coord) )
    {
        return true;
    }

    return false;
}



/**
 * Gets the shortest Manhattan distance route from player position
 * through each of the specified points .*/
cost_t ShortestMovesThroughPoints( const Coord& player,
                                   const Gears& gears,
                                   gears_bitfield_t gears_bitfield,
                                   const Coord& exit )
{
    cost_t shortest_cnt;

    Coords coords;
    coords.push_back( player );
    int size = (int)gears.size();
    for (int i=0; i<size; ++i)
    {
        if (gears_bitfield & ((gears_bitfield_t)1 << i))
        {
            coords.push_back( gears[i] );
        }
    }
    coords.push_back( exit );
    
    shortest_cnt = MovesThroughPoints( coords );
    for (size_t i=1; i<coords.size()-1; i++)
    {
        for (size_t j=i+1; j<coords.size()-1; j++)
        {
            Coord temp = coords[i];
            coords[i] = coords[j];
            coords[j] = temp;
            cost_t move_cnt = MovesThroughPoints( coords );
            shortest_cnt = move_cnt < shortest_cnt ? move_cnt : shortest_cnt;
        }
    }

    return shortest_cnt;
}



/**
   \brief Calculate the smallest number of moves through all coordinates.
   \details The first element in coords is the player position and the last is
            the exit. Any elements in between are gears. The cost is calculated
            for each permutation of gear ordering and the minimum is returned.
   \param[in] coords Player, gear(s), exit.
   \returns The minimum cost from player, through gears, to exit without any
            regard for obstacles.
 */
cost_t MovesThroughPoints( const Coords& coords )
{
    cost_t move_cnt = 0;
    int x_cnt;
    int y_cnt;
    int x1, y1, x2, y2;

    int sz = (int)coords.size();
    for (int i=0; i<sz-1; i++)
    {
        int j = i+1;
        x1 = (int)coords[i].x;
        y1 = (int)coords[i].y;
        x2 = (int)coords[j].x;
        y2 = (int)coords[j].y;
        x_cnt = x2 - x1;
        y_cnt = y2 - y1;
        x_cnt = x_cnt < 0 ? -x_cnt : x_cnt;
        y_cnt = y_cnt < 0 ? -y_cnt : y_cnt;
        move_cnt += (x_cnt + y_cnt);
#if 0
        printf("(%d,%d) to (%d,%d) %d\n", x1,y1,x2,y2,move_cnt);
#endif
    }
    
    return move_cnt;
}



/**
   \brief Check if gear exists at map coordinate.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool IsGear(const charmap& cmap, const Coord& coord)
{
    return cmap[coord.y][coord.x] == GEAR;
}



/**
   \brief Check if exit exists at map coordinate.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool IsExit(const charmap& cmap, const Coord& coord)
{
    return cmap[coord.y][coord.x] == EXIT;
}



/**
   \brief Check if box is above coord and it can move up.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool CanBoxMoveUp(const charmap& cmap, const Coord& coord)
{
    if (coord.y > 1)
    {
        char c1 = cmap[coord.y-1][coord.x];
        char c2 = cmap[coord.y-2][coord.x];
        return IS_BOX_MOVE(c1, c2);
    }
    return false;
}



/**
   \brief Check if box is below coord and it can move down.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool CanBoxMoveDown(const charmap& cmap, const Coord& coord)
{
    if (coord.y < HEIGHT-2)
    {
        char c1 = cmap[coord.y+1][coord.x];
        char c2 = cmap[coord.y+2][coord.x];
        return IS_BOX_MOVE(c1, c2);
    }
    return false;
}



/**
   \brief Check if box is left of coord and it can move left.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool CanBoxMoveLeft(const charmap& cmap, const Coord& coord)
{
    if (coord.x > 1)
    {
        char c1 = cmap[coord.y][coord.x-1];
        char c2 = cmap[coord.y][coord.x-2];
        return IS_BOX_MOVE(c1, c2);
    }
    return false;
}



/**
   \brief Check if box is right of coord and it can move right.
   \param[in] cmap 2D character array representation of level state.
   \param[in] coord Coordinate to check.
 */
bool CanBoxMoveRight(const charmap& cmap, const Coord& coord)
{
    if (coord.x < WIDTH-2)
    {
        char c1 = cmap[coord.y][coord.x+1];
        char c2 = cmap[coord.y][coord.x+2];
        return IS_BOX_MOVE(c1, c2);
    }
    return false;
}


enum ActionType
{
  ACTION_TYPE_PICKUP_GEAR,
  ACTION_TYPE_EXIT,
  ACTION_TYPE_MOVE_BOX_UP,
  ACTION_TYPE_MOVE_BOX_DOWN,
  ACTION_TYPE_MOVE_BOX_LEFT,
  ACTION_TYPE_MOVE_BOX_RIGHT
};

struct Action
{
  ActionType type_;
  vector<char> path_;
  Coordinate<uint8_t> destination_;
};

#define MAX_FSCORE 200

void astar(vector<vector<char> >& charmap, Heuristic& heuristic)
{
    fprintf(stderr, "making level\n");
    Level level = Level::MakeLevel(charmap);
    fprintf(stderr, "making start node\n");
    Node* start = Node::MakeStartNode(level, heuristic);
    
    // The set of nodes already evaluated
    set<Node*, NodeCompare> closed_set;
    
    // The current set of nodes that are not evaluated yet.
    // Initially, only the start node is known.
    set<Node*, NodeCompare> open_set;
    fprintf(stderr, "openset insert\n");
    open_set.insert(start);
    
    vector<list<Node*> > openset_fscore_nodes;
    fprintf(stderr, "resize\n");
    openset_fscore_nodes.resize(MAX_FSCORE);
    if (start->fscore() < MAX_FSCORE)
    {
        openset_fscore_nodes[start->fscore()].push_back(start);
    }
    
}

} // namespace boxedin
