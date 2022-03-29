#ifndef BOXED_IN_LEVEL
#define BOXED_IN_LEVEL

#include <map>
#include <vector>
#include "boxedintypes.h"
//#include "Node.h"

using namespace std;

namespace boxedin
{

class Node;

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

  vector<vector<char> > MakeFloodFillMap(const Node& node, bool draw_player) const;

  vector<vector<char> > Render() const;

  void TryPickupGear();
  void MoveUp();
  void MoveDown();
  void MoveLeft();
  void MoveRight();

  static bool CanMoveUp(std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
  static bool CanMoveDown(std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
  static bool CanMoveLeft(std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
  static bool CanMoveRight(std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
  
  static Level MakeLevel(const string& levelString);
  static Level MakeLevel(const vector<vector<char> >& charmap);
  static int GearsLeft(const vector<vector<char> >& charmap);
};

bool is_box(const std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
bool is_walkable(const std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
bool is_switch(const std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
bool can_flood(const std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);
bool can_hold_box(const std::vector<std::vector<char> >& charmap, uint8_t x, uint8_t y);

} // namespace boxedin

#endif
