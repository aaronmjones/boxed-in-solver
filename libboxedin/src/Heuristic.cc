#include "boxedintypes.h" // cost_t
#include "FloodFillNode.h"
#include "Heuristic.h"
#include "Node.h"

using namespace boxedin;

cost_t ShortestManhattenDistanceThroughGearsToExitHeuristic::cell_to_cell_dist(size_t cell1, size_t cell2)
{
    cost_t *cost_ptr = tile_to_tile_cost_table.Cost(cell1, cell2);
    if (*cost_ptr != COST_UNKNOWN)
    {
        return *cost_ptr;
    }

    vector<vector<char> > floor_plan = level.floor_plan_;
    std::queue<FloodFillNode> flood_fill_queue;
    Coord coord1(cell1 % floor_width, cell1 / floor_width);
    Coord coord2(cell2 % floor_width, cell2 / floor_width);
    cost_t cost = COST_INFINITY;
    
    // Queue of spaces to fill is initially the current player position
    flood_fill_queue.push( FloodFillNode(coord1) );

    while (!flood_fill_queue.empty())
    {
        FloodFillNode ffnode = flood_fill_queue.front();
        Coord& coord = ffnode.coord;

        if (coord == coord2)
        {
            cost = (cost_t)ffnode.path.size();
            break;
        }
        
        // Skip node if it's position has been filled
        if (floor_plan[coord.y][coord.x] == '-')
        {
            flood_fill_queue.pop();
            continue;
        }

        // Mark current point as filled
        floor_plan[coord.y][coord.x] = '-';

        //// Add neighbor nodes in the 4 directions ////

        // UP
        if ( (coord.y > 0) && (floor_plan[coord.y-1][coord.x] == ' ') )
        {
            FloodFillNode up( ffnode ); // copy of node
            up.path.push_back(ENCODED_PATH_DIRECTION_UP);
            up.coord.y--;
            flood_fill_queue.push( up );
        }
        // DOWN
        if ( (coord.y < floor_height-1) && (floor_plan[coord.y+1][coord.x] == ' ') )
        {
            FloodFillNode down( ffnode ); // copy of node
            down.path.push_back(ENCODED_PATH_DIRECTION_DOWN);
            down.coord.y++;
            flood_fill_queue.push( down );
        }
        // LEFT
        if ( (coord.x > 0) && (floor_plan[coord.y][coord.x-1] == ' ') )
        {
            FloodFillNode left( ffnode ); // copy of node
            left.path.push_back(ENCODED_PATH_DIRECTION_LEFT);
            left.coord.x--;
            flood_fill_queue.push( left );
        }
        // RIGHT
        if ( (coord.x < floor_width-1) && (floor_plan[coord.y][coord.x+1] == ' ') )
        {
            FloodFillNode right( ffnode ); // copy of node
            right.path.push_back(ENCODED_PATH_DIRECTION_RIGHT);
            right.coord.x++;
            flood_fill_queue.push( right );
        }
    }

    *cost_ptr = cost;
    return cost;
}


cost_t ShortestManhattenDistanceThroughGearsToExitHeuristic::get_hscore(size_t cell, uint16_t gears_bitfield)
{
#if 0
    fprintf(stderr, "cell=%lu num_gears=%lu gears=0x%04x\n", cell, num_gears, gears_bitfield);
#endif
    cost_t *cost_ptr = &(hscore_table[cell * (1 << num_gears) + gears_bitfield]);
    if (*cost_ptr != COST_UNKNOWN)
    {
#if 0
        fprintf(stderr, "    cost already known %p (%d)\n", cost_ptr, *cost_ptr);
#endif
        return *cost_ptr;
    }


    if (gears_bitfield == 0)
    {
        size_t exit_cell = ((level.exit_coord_.y * floor_width) + level.exit_coord_.x);
        *cost_ptr = cell_to_cell_dist(cell, exit_cell);
#if 0
        fprintf(stderr, "    robot-to-exit distance %lu..%lu is %d\n", cell, exit_cell, *cost_ptr);
#endif
        return *cost_ptr;
    }

    cost_t best_cost = COST_INFINITY;
    for (size_t i = 0; i < num_gears; i++)
    {
        uint16_t checkbit = 1 << i;
        if (checkbit & gears_bitfield)
        {
            const Coord& gear_coord = level.gear_coords_[i];
            size_t gear_cell = ((gear_coord.y * floor_width) + gear_coord.x);
            cost_t cost = cell_to_cell_dist(cell, gear_cell) + get_hscore(gear_cell, gears_bitfield & ~checkbit);
            best_cost = (cost < best_cost) ? cost : best_cost;
        }
    }

    *cost_ptr = best_cost;
    return *cost_ptr;
}


// virtual
cost_t ShortestManhattenDistanceThroughGearsToExitHeuristic::get_hscore(const Node& node)
{
    size_t robot_cell = ((node.player_coord_.y * floor_width) + node.player_coord_.x);
    uint16_t gears_bitfield = node.gear_descriptor_.bitfield;
    cost_t hscore = get_hscore(robot_cell, gears_bitfield);
#if 0
    fprintf(stderr, "getting hscore for robot at (%u,%u)...%d\n", node.player_coord_.x, node.player_coord_.y, hscore);
#endif
    return hscore;
}
