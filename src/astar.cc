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
#include "FloodFillNode.h"

#include <iostream>
#include <boxedinio.h>

using namespace std;

namespace boxedin {


#define MAX_FSCORE 200


// The set of nodes already evaluated
set<Node*, NodeCompare> closed_set;

// The current set of nodes that are not evaluated yet.
// Initially, only the start node is known.
set<Node*, NodeCompare> open_set;

vector<list<Node*> > openset_fscore_nodes;

Node* get_next_best_fscore_node(cost_t current_fscore)
{
    Node* node = NULL;
    cost_t fscore = current_fscore;
    while ( (node == NULL) && (fscore < MAX_FSCORE) )
    {
        list<Node*>& nodes = openset_fscore_nodes[fscore];
        if ( nodes.empty() )
        {
#if 0
            fprintf(stderr, "empty node list at index %d\n", (int)fscore);
#endif
            fscore++;
            continue;
        }
        node = nodes.front();
        nodes.pop_front();
    }
    return node;
}

bool is_box( const vector<vector<char> >& level_map, uint8_t x, uint8_t y )
{
    return level_map[y][x] == '+';
}

bool is_fillable( const vector<vector<char> >& level_map, uint8_t x, uint8_t y )
{
    switch (level_map[y][x])
    {
    case ' ':
    case 'r':
    case 'y':
    case 'g':
    case 'b':
    case '*':
    case '@':
        return true;
    default:
        return false;
    }
}

bool can_flood( const vector<vector<char> >& level_map, uint8_t x, uint8_t y )
{
    switch (level_map[y][x])
    {
    case ' ':
    case 'r':
    case 'y':
    case 'g':
    case 'b':
        return true;
    default:
        return false;
    }
}

bool can_hold_box( const vector<vector<char> >& level_map, uint8_t x, uint8_t y )
{
    switch (level_map[y][x])
    {
    case ' ':
    case '-':
    case 'r':
    case 'y':
    case 'g':
    case 'b':
        return true;
    default:
        return false;
    }
}

list<Action> find_actions(const Level& level, const Node& node)
{
    list<Action> actions;
    uint8_t floor_width = (uint8_t)level.floor_plan_[0].size();
    uint8_t floor_height = (uint8_t)level.floor_plan_.size();
    bool draw_player = false;
    vector<vector<char> > level_map = level.Map( node, draw_player ); // TODO: rename to level.MakeFloodFillMap()?
#if 0
    fprintf(stderr, "finding actions for:\n");
    PrintCharMapInColor(cerr, level_map);
#endif
    //// Flood fill to find all action points ////

    // TODO: should you consider the tile the robot is currently on here? it seems like
    //       stepping off of a switch should be considered an "action"
    
    // Queue of spaces to fill is initially the current player position
    queue<FloodFillNode> flood_fill_queue;
    flood_fill_queue.push( FloodFillNode(node.player_coord_) );

    while ( !flood_fill_queue.empty() )
    {
        FloodFillNode ffnode = flood_fill_queue.front();
        Coord& coord = ffnode.coord;
#if 0
        fprintf(stderr, "flood fill map:\n");
        PrintCharMapInColor(cerr, level_map);
#endif
        // Skip node if it's position has been filled
        if (level_map[coord.y][coord.x] == '-')
        {
            flood_fill_queue.pop();
            continue;
        }

        // If it's an action point, add it to the results

        if ( level_map[coord.y][coord.x] == '@' ) // exit
        {
            actions.push_back( Action(ACTION_TYPE_EXIT, ffnode.path, ffnode.coord) );            
        }
        else if ( level_map[coord.y][coord.x] == '*' ) // gear
        {
            actions.push_back( Action(ACTION_TYPE_PICKUP_GEAR, ffnode.path, ffnode.coord) );            
        }
        else
        {
            // is there a box directly above that can be pushed?
            if ( coord.y > 1 &&
                 is_box( level_map, coord.x, coord.y-1 ) &&
                 can_hold_box( level_map, coord.x, coord.y-2 ) )
            {
                Action action( ACTION_TYPE_MOVE_BOX_UP, ffnode.path, ffnode.coord );
                action.path.push_back( ENCODED_PATH_DIRECTION_UP );
                action.point.y--;
                actions.push_back( action  );            
            }
            // is there a box directly below that can be pushed?
            if ( coord.y < floor_height-2 &&
                 is_box( level_map, coord.x, coord.y+1 ) &&
                 can_hold_box( level_map, coord.x, coord.y+2 ) )
            {
                Action action( ACTION_TYPE_MOVE_BOX_DOWN, ffnode.path, ffnode.coord );
                action.path.push_back( ENCODED_PATH_DIRECTION_DOWN );
                action.point.y++;
                actions.push_back( action  );            
            }
            // is there a box directly left that can be pushed?
            if ( coord.x > 1 &&
                 is_box( level_map, coord.x-1, coord.y ) &&
                 can_hold_box( level_map, coord.x-2, coord.y ) )
            {
                Action action( ACTION_TYPE_MOVE_BOX_LEFT, ffnode.path, ffnode.coord );
                action.path.push_back( ENCODED_PATH_DIRECTION_LEFT );
                action.point.x--;
                actions.push_back( action  );            
            }
            // is there a box directly right that can be pushed?
            if ( coord.x < floor_width-2 &&
                 is_box( level_map, coord.x+1, coord.y ) &&
                 can_hold_box( level_map, coord.x+2, coord.y ) )
            {
                Action action( ACTION_TYPE_MOVE_BOX_RIGHT, ffnode.path, ffnode.coord );
                action.path.push_back( ENCODED_PATH_DIRECTION_RIGHT );
                action.point.x++;
                actions.push_back( action  );            
            }
        }

        if ( can_flood(level_map, coord.x, coord.y) )
        {
            //// Add neighbor nodes in the 4 directions ////
            
            // UP
            if ( (coord.y > 0) && is_fillable(level_map, coord.x, coord.y-1) )
            {
                FloodFillNode up( ffnode ); // copy of node
                up.path.push_back( ENCODED_PATH_DIRECTION_UP );
                up.coord.y--;
                flood_fill_queue.push( up );
            }
            // DOWN
            if ( (coord.y < floor_height-1) && is_fillable(level_map, coord.x, coord.y+1) )
            {
                FloodFillNode down( ffnode ); // copy of node
                down.path.push_back( ENCODED_PATH_DIRECTION_DOWN );
                down.coord.y++;
                flood_fill_queue.push( down );
            }
            // LEFT
            if ( (coord.x > 0) && is_fillable(level_map, coord.x-1, coord.y) )
            {
                FloodFillNode left( ffnode ); // copy of node
                left.path.push_back( ENCODED_PATH_DIRECTION_LEFT );
                left.coord.x--;
                flood_fill_queue.push( left );
            }
            // RIGHT
            if ( (coord.x < floor_width-1) && is_fillable(level_map, coord.x+1, coord.y) )
            {
                FloodFillNode right( ffnode ); // copy of node
                right.path.push_back( ENCODED_PATH_DIRECTION_RIGHT );
                right.coord.x++;
                flood_fill_queue.push( right );
            }
        }

        // Mark current point as filled
        level_map[coord.y][coord.x] = '-';
        
    } // end while
    
    return actions;
}

int is_boxing_char(char c)
{
    switch (c)
    {
    case '\'':
    case 'x':
    case '+':
        return 1;
    default:
        return 0;
    }
}

bool boxed_in(const Coord& coord, vector<vector<char> >& level_map)
{
    uint8_t x = coord.x;
    uint8_t y = coord.y;
    uint8_t floor_width = (uint8_t)level_map[0].size();
    uint8_t floor_height = (uint8_t)level_map.size();
    
    int nw = (x > 0 && y > 0) ? is_boxing_char(level_map[y-1][x-1]) : 0;
    int n = (y > 0) ? is_boxing_char(level_map[y-1][x]) : 0;
    int ne = (x < floor_width-1 && y > 0) ? is_boxing_char(level_map[y-1][x+1]) : 0;
    int e = (x < floor_width-1) ? is_boxing_char(level_map[y][x+1]) : 0;
    int se = (x < floor_width-1 && y < floor_height-1) ? is_boxing_char(level_map[y+1][x+1]) : 0;
    int s = (y < floor_height-1) ? is_boxing_char(level_map[y+1][x]) : 0;
    int sw = (x > 0 && y < floor_height-1) ? is_boxing_char(level_map[y+1][x-1]) : 0;
    int w = (x > 0) ? is_boxing_char(level_map[y][x-1]) : 0;
#if 0
    fprintf(stderr, "-----------\n");
    fprintf(stderr, "  %d%d%d\n", nw, n, ne);
    fprintf(stderr, "  %d %d\n", w, e);
    fprintf(stderr, "  %d%d%d\n", sw, s, se);
#endif
    if ( (n + s + e + w == 4) &&
         (nw + ne + se + sw >= 3) )
    {
        return true; // char c is boxed in!
    }
    return false;
}

bool is_unsolvable(const Level& level, Node& node, vector<vector<char> >& level_map)
{
    if ( boxed_in(level.exit_coord_, level_map) )
    {
        return true;
    }

    int sz = (int)level.gear_coords_.size();
    for (int i = 0; i < sz; i++)
    {
        uint16_t bitmask = (uint16_t)1 << i;
        if ( ((bitmask & node.gear_descriptor_.bitfield) == bitmask) )
        {
            if ( boxed_in(level.gear_coords_[i], level_map) )
            {
                return true;
            }
        }
    }
    return false;
}

list<Node*> generate_successors(const Level& level, Heuristic& heuristic, Node& node)
{
    list<Node*> successors;
    list<Action> actions = find_actions( level, node );

#if 1
    bool draw_player = true;
    vector<vector<char> > level_map = level.Map( node, draw_player );
    if ( is_unsolvable(level, node, level_map) )
    {
#if 0
        fprintf(stderr, "pruning unsolvable level---------------------------\n");
        PrintCharMapInColor(cerr, level_map);
#endif
        return successors;
    }
#endif

    list<Action>::iterator it;
    for (it=actions.begin(); it!=actions.end(); ++it)
    {
        Node* successor = new Node( level, heuristic, node, *it );
        successors.push_back( successor );
    }

    return successors;
}

SearchResult astar(Level& level, Heuristic& heuristic)
{
    SearchResult result;
    Node* start = Node::MakeStartNode(level, heuristic);
    
    open_set.insert(start);
    openset_fscore_nodes.resize(MAX_FSCORE);
    if (start->fscore() < MAX_FSCORE)
    {
        openset_fscore_nodes[start->fscore()].push_back(start);
    }

    Node* node = NULL;
    cost_t fscore = start->fscore();
    
    while ( (node = get_next_best_fscore_node(fscore)) != NULL )
    {
#if 0
        if ( fscore != node->fscore() )
        {
          fprintf(stderr, "fscore is %d: %d + %d\n", fscore, node->gscore_, node->hscore_);
        }
#endif
        fscore = node->fscore();

        if (node->better_gscore_found_)
        {
#if 0
            fprintf(stderr, "dropping a node (better gscore was found)\n");
#endif
            delete node;
            continue;
        }

        if ( node->IsGoal(level) )
        {
            result.SetSucceeded( node, open_set.size(), closed_set.size() );
            return result;
        }

        open_set.erase(node);
        closed_set.insert(node);

        list<Node*> successors = generate_successors(level, heuristic, *node);

#if 0
        fprintf(stderr, "%lu successors found\n", successors.size());
#endif
        for ( list<Node*>::iterator it = successors.begin(); it != successors.end(); ++it )
        {
            Node* successor = *it;

#if 0
            bool draw_player = true;
            vector<vector<char> > level_map = level.Map( *successor, draw_player );
            fprintf(stderr, "successor -------------------------------------------\n");
            PrintCharMapInColor(cerr, level_map);
#endif
            
            // successor already in closed set?
            set<Node*, NodeCompare>::iterator it_closed = closed_set.find(successor);
            if ( it_closed != closed_set.end() )
            {
#if 0
                fprintf(stderr, "dropping node already in closedset\n");
#endif
                delete successor;
                continue;
            }

            // successor already in open set?
            set<Node*, NodeCompare>::iterator it_open = open_set.find(successor);
            if ( it_open != open_set.end() )
            {
                if ( successor->gscore_ < (*it_open)->gscore_)
                {
#if 0
                    fprintf(stderr, "found better gscore!\n");
#endif
                    // Instead of removing the old node from the open_set and openset_fscore_nodes,
                    // just flag it for deletion.
                    (*it_open)->better_gscore_found_ = true;
                }
                else
                {
#if 0
                    fprintf(stderr, "dropping node already in openset\n");
#endif
                    delete successor;
                    continue;
                }
            }

            // new search node!!!
            if ( successor->fscore() < MAX_FSCORE)
            {
#if 0
                fprintf(stderr, " inserting successor with fscore=%d\n", successor->fscore());
#endif
                open_set.insert( successor );
                openset_fscore_nodes[successor->fscore()].push_back( successor );
            }
            else
            {
#if 0
                fprintf(stderr, "warning: dropping node with fscore %d (>%d)\n",
                        successor->fscore(), MAX_FSCORE);
#endif
                delete successor;
                continue;
            }            
        } // end for (successors)

    } // end while

    //TODO: delete heap memory
    result.SetFailed(open_set.size(), closed_set.size());
    return result;
}

} // namespace boxedin
