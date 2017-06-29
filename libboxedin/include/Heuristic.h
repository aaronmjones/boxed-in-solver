#ifndef BOXED_IN_HEURISTIC
#define BOXED_IN_HEURISTIC

#include "boxedintypes.h"
#include "Level.h"
#include "Node.h"

using namespace std;

namespace boxedin
{

class Node; // forward
    
struct Heuristic
{
    virtual cost_t calculate(const Level& level, const Node& node) const = 0;
};

struct ShortestManhattenDistanceThroughGearsToExitHeuristic : public Heuristic
{
    //   0 1 2 3 4 5 6
    // 0 a b c d e f g
    // 1 - h i j k l m
    // 2 - - n o p q r
    // 3 - - - s t u v
    // 4 - - - - w x y
    // 5 - - - - - z A
    // 6 - - - - - - B
    
    // Tile to tile cost, taking walls into account but ignoring gates and boxes.
    // Index by 
    cost_t* tile_to_tile_cost;

    // cost_table contains the cost from a tile, through
    vector<vector<cost_t> > cost_table;
    
    ShortestManhattenDistanceThroughGearsToExitHeuristic(size_t num_rows, size_t num_cols, size_t num_gears)
    {
        size_t num_tiles = num_rows * num_cols;
        size_t sz = num_tiles * num_tiles; // FIXME: table is 2x size it needs to be!
        tile_to_tile_cost = new cost_t[sz];
        for (size_t row = 0; row < num_rows; row++)
        {
            for (size_t col = 0; col < num_cols; col++)
            {
                if (row == col)
                {
                    tile_to_tile_cost[row * num_cols + col] = 0;
                }
                else
                {
                    tile_to_tile_cost[row * num_cols + col] = COST_UNKNOWN;
                }
            }
        }
    }
    
    ~ShortestManhattenDistanceThroughGearsToExitHeuristic()
    {
        delete tile_to_tile_cost;
    }

    virtual cost_t calculate(const Level& level, const Node& node) const
    {
        return 0;//TODO
    }
};


} // namespace boxedin


#endif // BOXED_IN_HEURISTIC
