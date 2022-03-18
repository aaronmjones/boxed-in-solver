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
    virtual cost_t get_hscore(const Node& node) = 0;
};


//             x
//
//       0 1 2 3 4 5 6
//     0 a b c d e f g
//     1 b h i j k l m
//     2 c i n o p q r
//  y  3 d j o s t u v
//     4 e k p t w x y
//     5 f l q u x z A
//     6 g m r v y A B

//             x
//
//       0 1 2 3 4 5 6
//     0 a b c d e f g
//     1 - h i j k l m
//     2 - - n o p q r
//  y  3 - - - s t u v
//     4 - - - - w x y
//     5 - - - - - z A
//     6 - - - - - - B
//
//     rect_offset = x * table_width + y;
//     table_offset = rect_offset - (∑ 1 to y);
//     table_offset = rect_offset - ((y * (y + 1)) / 2);

class SymmetricCostTable
{
public:
    cost_t* table;
    size_t table_width;
    
    SymmetricCostTable(size_t table_width)
        : table_width(table_width)
    {
        size_t n = (table_width * (table_width + 1)) / 2;
        table = new cost_t[n];
        
        for (size_t row = 0; row < table_width; row++)
        {
            for (size_t col = 0; col < table_width; col++)
            {
                if (row == col)
                {
                    Cost(row, col, 0);
                }
                else
                {
                    Cost(row, col, COST_UNKNOWN);
                }
            }
        }
    }

    ~SymmetricCostTable()
    {
        delete table;
    }

    size_t get_memory_offset(size_t x, size_t y)
    {
        size_t table_offset;
        size_t memory_offset;
        if (y > x)
        {
            size_t temp = x;
            x = y;
            y = temp;
        }
        table_offset = (y * table_width) + x;
        memory_offset = table_offset - ((y * (y + 1)) / 2);
        return memory_offset;
    }
    
    void Cost(size_t x, size_t y, cost_t cost)
    {
        size_t memory_offset = get_memory_offset(x, y);
        table[memory_offset] = cost;
    }

    cost_t* Cost(size_t x, size_t y)
    {
        size_t memory_offset = get_memory_offset(x, y);
        cost_t* cost = &table[memory_offset];
        return cost;
    }
private:
    SymmetricCostTable(SymmetricCostTable& other); // no copy
    SymmetricCostTable& operator=(SymmetricCostTable& other); // no copy
};


struct ShortestDistanceThroughGearsToExitHeuristic : public Heuristic
{
    const Level& level;

    size_t floor_width;

    size_t floor_height;
    
    size_t num_tiles;

    size_t num_gears;
    
    // Cost from tile to tile, taking walls into account but ignoring gates and boxes.
    // \note This table is not indexed by x,y coords of 2 tiles, but rather the
    // tile index of 2 tiles where tile index is y * row_width + x.
    //
    // size_t tile1 = y1 * row_width + x1;
    // size_t tile2 = y2 * row_width + x2;
    // cost_t cost = tile_to_tile_cost_table.Cost(tile1, tile2);
    SymmetricCostTable tile_to_tile_cost_table;

    // Cost from tile through gears to exit.
    //
    // Index by tile offset and gears bitfield.
    //
    // size_t player_offset = player_y * row_width + player_x
    // cost_t hscore = hscore_table[player_offset * num_gears + gears_bitfield];
    //
    // \note If there are 140 tiles and 12 gears, the table size is
    //       140 * (2^12) = 573440!
    cost_t *hscore_table;
    
    ShortestDistanceThroughGearsToExitHeuristic(const Level& level)
        : level(level)
        , floor_width(level.floor_plan_[0].size())
        , floor_height(level.floor_plan_.size())
        , num_tiles(floor_width * floor_height)
        , num_gears(level.gear_coords_.size())
        , tile_to_tile_cost_table(num_tiles)
    {
        size_t sz = num_tiles * (1 << num_gears);
#if 1
        fprintf(stderr, "floor_width %lu floor_height %lu num_tiles %lu num_gears %lu\n", floor_width, floor_height, num_tiles, num_gears);
        fprintf(stderr, "allocating hscore_table of size %lu ===========================================\n", sz);
#endif
        hscore_table = new cost_t[sz];
        for (size_t i = 0; i < sz; i++)
        {
            hscore_table[i] = COST_UNKNOWN;
        }
    }
    
    ~ShortestDistanceThroughGearsToExitHeuristic()
    {
        delete hscore_table;
    }

    cost_t cell_to_cell_dist(size_t cell1, size_t cell2);
    cost_t get_hscore(size_t cell, uint16_t gears_bitfield);
    virtual cost_t get_hscore(const Node& node);
};


} // namespace boxedin


#endif // BOXED_IN_HEURISTIC
