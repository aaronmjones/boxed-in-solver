#include "Level.h"
#include "Node.h"

namespace boxedin
{

vector<vector<char> > Level::Map(const Node& node, bool draw_player) const
{
    int sz = 0;
    int floor_width = (int)floor_plan_[0].size();
    vector<vector<char> > level_map = floor_plan_;
    
    // set boxes
    sz = (int)node.box_descriptor_.bitfields.size();
    for (int i = 0; i < sz; i++)
    {
        uint64_t bitfield = node.box_descriptor_.bitfields[i];
        for (int bit_index = 0; bit_index < node.box_descriptor_.kBitfieldWidth; bit_index++ )
        {
            uint64_t bitmask = (uint64_t)1 << bit_index;
            if ( (bitfield & bitmask) == bitmask )
            {
                int tile_index = i * node.box_descriptor_.kBitfieldWidth + bit_index;
                Coord box( tile_index % floor_width, tile_index / floor_width );
                level_map[box.y][box.x] = '+';
            }
        }
    }

    // set gears
    sz = (int)gear_coords_.size();
    for (int i = 0; i < sz; i++)
    {
        if ( node.gear_descriptor_.bitfield & ((uint64_t)1 << i) )
        {
            const Coord& coord = gear_coords_[i];
            level_map[coord.y][coord.x] = '*';
        }
    }

    // set player
    if ( draw_player )
    {
        level_map[node.player_coord_.y][node.player_coord_.x] = 'p';
    }
    
    // set switches and gates
    map<Color, pair<Coord, Coord> >::const_iterator it;
    for (it = switch_gate_pairs_.begin(); it != switch_gate_pairs_.end(); ++it)
    {
        Color c = it->first;
        Coord sw = it->second.first;
        Coord gate = it->second.second;
        if (level_map[sw.y][sw.x] == ' ')
        {
            // switch not active; draw switch and gate
            level_map[sw.y  ][sw.x  ] = COLOR_TO_SWITCH_CHAR(c);
            level_map[gate.y][gate.x] = COLOR_TO_GATE_CHAR(c);
        }               
    }

    return level_map;
}

} // namespace boxedin
