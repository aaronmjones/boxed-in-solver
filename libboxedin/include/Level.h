#ifndef BOXED_IN_LEVEL
#define BOXED_IN_LEVEL

#include <map>
#include <vector>
#include "boxedintypes.h"


using namespace std;

namespace boxedin
{

class Level
{
public:
  // 2D char vector of space, wall and floor only
  vector<vector<char> > floor_plan_;

  // The initial player coordinate
  Coordinate<uint8_t> player_coord_;

  // The exit coordinate
  Coordinate<uint8_t> exit_coord_;

  // The initial box coordinates
  vector<Coordinate<uint8_t> > box_coords_;

  // The gear coordinates
  vector<Coordinate<uint8_t> > gear_coords_;

  // The floor switch and corresponding gate for each color
  map<Color, pair<Coord, Coord> > switch_gate_pairs_;

  static Level MakeLevel(vector<vector<char> >& charmap)
    {
      Level level;
      int player_coords_found = 0;
      int exit_coords_found = 0;
      int invalid_chars_found = 0;

      // Create the floor plan; the level elements that can change become
      // spaces (' ') here.
      level.floor_plan_ = charmap;
      for (size_t y = 0; y < charmap.size(); y++)
      {
        for (size_t x = 0; x < charmap[y].size(); x++)
        {
          char c = charmap[y][x];
          switch (c)
          {
          case '+':  // box
          case '*':  // gear
          case 'p':  // player
          case 'r':  // switch (red)
          case 'g':  // switch (green)
          case 'b':  // switch (blue)
          case 'y':  // switch (yellow)
          case 'R':  // gate (red)
          case 'G':  // gate (green)
          case 'B':  // gate (blue)
          case 'Y':  // gate (yellow)
            level.floor_plan_[y][x] = ' ';
            break;
          default:   // invalid char
            break;
          }
        }
      }

      // Get coordinates of player, exit, gears, etc.
      for (size_t y = 0; y < charmap.size(); y++)
      {
        for (size_t x = 0; x < charmap[y].size(); x++)
        {
          char c = charmap[y][x];
          switch (c)
          {
          case '\'': // space
          case ' ':  // floor
          case 'x':  // wall
            break;
          case '+':  // box
            level.box_coords_.push_back(Coord(x,y));
            break;
          case '*':  // gear
            level.gear_coords_.push_back(Coord(x,y));
            break;
          case '@':  // exit
            level.exit_coord_ = Coord(x,y);
            exit_coords_found++;
            break;
          case 'p':  // player
            level.player_coord_ = Coord(x,y);
            player_coords_found++;
            break;
          case 'r':  // switch (red)
            level.switch_gate_pairs_[RED].first = Coord(x,y);
            break;
          case 'g':  // switch (green)
            level.switch_gate_pairs_[GREEN].first = Coord(x,y);
            break;
          case 'b':  // switch (blue)
            level.switch_gate_pairs_[BLUE].first = Coord(x,y);
            break;
          case 'y':  // switch (yellow)
            level.switch_gate_pairs_[YELLOW].first = Coord(x,y);
            break;
          case 'R':  // gate (red)
            level.switch_gate_pairs_[RED].second = Coord(x,y);
            break;
          case 'G':  // gate (green)
            level.switch_gate_pairs_[GREEN].second = Coord(x,y);
            break;
          case 'B':  // gate (blue)
            level.switch_gate_pairs_[BLUE].second = Coord(x,y);
            break;
          case 'Y':  // gate (yellow)
            level.switch_gate_pairs_[YELLOW].second = Coord(x,y);
            break;
          default:   // invalid char
            invalid_chars_found++;
            break;
          }
        }
      }

      return level;
    }
};

} // namespace boxedin

#endif
