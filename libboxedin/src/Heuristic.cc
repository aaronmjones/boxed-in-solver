#include "boxedintypes.h" // cost_t
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



#if 0
    vector<vector<char> > floor_plan = level.floor_plan_;
    std::queue<FloodFillNode> flood_fill_queue;

    // Queue of spaces to fill is initially the current player position
    flood_fill_queue.push( FloodFillNode(Coord()) );

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
#endif





    
    return 0; //TODO
}


cost_t ShortestManhattenDistanceThroughGearsToExitHeuristic::get_hscore(size_t cell, uint16_t gears_bitfield)
{
    cost_t *cost_ptr = &hscore_table[((cell * num_gears) + gears_bitfield)];
    if (*cost_ptr != COST_UNKNOWN)
    {
        return *cost_ptr;
    }


    if (gears_bitfield == 0)
    {
        size_t exit_cell = ((level.exit_coord_.y * floor_width) + level.exit_coord_.x);
        *cost_ptr = cell_to_cell_dist(cell, exit_cell);
        return *cost_ptr;
    }
    
    cost_t best_cost = COST_INFINITY;
    size_t num_gears = level.gear_coords_.size();
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
    return get_hscore(robot_cell, gears_bitfield);
}
