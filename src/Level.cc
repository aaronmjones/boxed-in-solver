#include "Level.h"
#include "Node.h"

namespace boxedin
{


// Create a 2D character "map" representation using the floor plan of the Level
// and the gear, box and player position from the Node. This function is used
// by the astar search.
vector<vector<char> > Level::MakeFloodFillMap(const Node& node, bool draw_player) const
{
  int sz = 0;
  int floor_width = (int)floor_plan_[0].size();
  vector<vector<char> > level_map = floor_plan_;
    
  // set boxes
  sz = (int)node.box_descriptor_.size;
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
#if 0
        fprintf(stderr, "box: tile_index=%d x=%u y=%u\n", tile_index, box.x, box.y);//TODO: remove
#endif
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

  // set exit
  level_map[exit_coord_.y][exit_coord_.x] = '@';
    
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


// Create a 2D character "map" representation of the Level. This function is
// used by the solution viewer.
vector<vector<char> > Level::Render() const
{
  int sz = 0;
  int floor_width = (int)floor_plan_[0].size();
  vector<vector<char> > level_map = floor_plan_;

  // set boxes
  for (auto box : box_coords_)
  {
    level_map[box.y][box.x] = '+';
  }

  // set gears
  for (auto gear : gear_coords_)
  {
      level_map[gear.y][gear.x] = '*';    
  }

  // set player
  level_map[player_coord_.y][player_coord_.x] = 'p';

  // set exit
  level_map[exit_coord_.y][exit_coord_.x] = '@';
    
  // set switches and gates
  map<Color, pair<Coord, Coord> >::const_iterator it;
  for (auto& kv : switch_gate_pairs_)
  {
    Color c = kv.first;
    Coord sw = kv.second.first;
    Coord gate = kv.second.second;
    if (level_map[sw.y][sw.x] == ' ')
    {
      // switch not active; draw switch and gate
      level_map[sw.y  ][sw.x  ] = COLOR_TO_SWITCH_CHAR(c);
      level_map[gate.y][gate.x] = COLOR_TO_GATE_CHAR(c);
    }
  }

  return level_map;
}

Level Level::MakeLevel(const string& levelString)
{
  // First make a 2D vector from the string
  vector<vector<char> > charMap;
  vector<char> charRow;
  for (const auto& c : levelString)
  {
    if (c == '\n')
    {
      charMap.push_back(charRow);
      charRow.clear();
    }
    else
    {
      charRow.push_back(c);
    }
  }

  return MakeLevel(charMap);
}


Level Level::MakeLevel(const vector<vector<char> >& charmap)
{
  Level level;
  int player_coords_found = 0;
  int exit_coords_found = 0;
  int invalid_chars_found = 0;
  uint8_t width;
  uint8_t height;

  // Create the floor plan; the level elements that can change become
  // spaces (' ') here.
  level.floor_plan_ = charmap;
  height = static_cast<uint8_t>(charmap.size());
  for (size_t y = 0; y < charmap.size(); y++)
  {
    width = static_cast<uint8_t>(charmap[y].size());
    for (size_t x = 0; x < width; x++)
    {
      char c = charmap[y][x];
      switch (c)
      {
      case '@':  // exit
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
      case '\'': // space
      case 'x':  // wall
        level.floor_plan_[y][x] = c;
        break;
      default:   // invalid char
        break;
      }
    }
  }

  // Get coordinates of player, exit, gears, etc.
  height = static_cast<uint8_t>(charmap.size());
  for (uint8_t y = 0; y < height; y++)
  {
    width = static_cast<uint8_t>(charmap[y].size());
    for (size_t x = 0; x < width; x++)
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


int Level::GearsLeft(const vector<vector<char> >& charmap)
{
  int gearsLeft = 0;
  for (const auto & row : charmap)
  {
    for (const auto & c : row)
    {
      if (c == GEAR)
        ++gearsLeft;
    }
  }
  return gearsLeft;
}


/**
   \brief Remove the gear at the current player coordinate.
 */
void Level::TryPickupGear()
{
  gear_coords_.erase(std::remove(gear_coords_.begin(), gear_coords_.end(), player_coord_), gear_coords_.end());
}


/**
   \brief Move player up, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void Level::MoveUp()
{
    // Move the player up
    player_coord_.y--;
    // If there's a box, move it up
    vector<Coordinate<uint8_t> >::iterator it = find(box_coords_.begin(), box_coords_.end(), player_coord_);
    if (it != box_coords_.end())
    {
        it->y--;
    }
}


/**
   \brief Move player down, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void Level::MoveDown()
{
    // Move the player_coord_ down
    player_coord_.y++;
    // If there's a box, move it down
    vector<Coordinate<uint8_t> >::iterator it = find(box_coords_.begin(), box_coords_.end(), player_coord_);
    if (it != box_coords_.end())
    {
        it->y++;
    }
}


/**
   \brief Move player left, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void Level::MoveLeft()
{
    // Move the player_coord_ left
    player_coord_.x--;
    // If there's a box, move it left
    vector<Coordinate<uint8_t> >::iterator it = find(box_coords_.begin(), box_coords_.end(), player_coord_);
    if (it != box_coords_.end())
    {
        it->x--;
    }
}


/**
   \brief Move player right, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void Level::MoveRight()
{
    // Move the player right
    player_coord_.x++;
    // If there's a box, move it right
    vector<Coordinate<uint8_t> >::iterator it = find(box_coords_.begin(), box_coords_.end(), player_coord_);
    if (it != box_coords_.end())
    {
        it->x++;
    }
}


bool Level::IsBoxAndCanMoveUp(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  return (y > 0 && is_box(charmap, x, y) && can_hold_box(charmap, x, y-1));
}


bool Level::IsBoxAndCanMoveDown(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  uint8_t floor_height = (uint8_t)charmap.size();
  return (y < floor_height - 1 && is_box(charmap, x, y) && can_hold_box(charmap, x, y + 1));
}


bool Level::IsBoxAndCanMoveLeft(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  return (x > 0 && is_box(charmap, x, y) && can_hold_box(charmap, x-1, y));
}


bool Level::IsBoxAndCanMoveRight(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  uint8_t floor_width = (uint8_t)charmap[0].size();
  return (x < floor_width - 1 && is_box(charmap, x, y) && can_hold_box(charmap, x+1, y));
}


bool Level::CanMoveUp(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  // Can walk up?
  if ((y > 0) && is_walkable(charmap, x, y-1))
    return true;
  // Can move box up?
  if (Level::IsBoxAndCanMoveUp(charmap, x, y-1))
    return true;
  return false;
}


bool Level::CanMoveDown(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  uint8_t floor_height = (uint8_t)charmap.size();
  // Can walk up?
  if ( (y < floor_height-1) && is_walkable(charmap, x, y+1) )
    return true;
  // Can move box up?
  if (Level::IsBoxAndCanMoveDown(charmap, x, y+1))
    return true;
  return false;
}


bool Level::CanMoveLeft(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  // Can walk up?
  if ((x > 0) && is_walkable(charmap, x-1, y))
    return true;
  // Can move box up?
  if (Level::IsBoxAndCanMoveLeft(charmap, x-1, y))
    return true;
  return false;
}


bool Level::CanMoveRight(vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
  uint8_t floor_width = (uint8_t)charmap[0].size();
  // Can walk up?
  if ((x < floor_width - 1) && is_walkable(charmap, x + 1, y))
    return true;
  // Can move box up?
  if (Level::IsBoxAndCanMoveRight(charmap, x+1, y))
    return true;
  return false;
}


bool is_box(const vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
    return charmap[y][x] == '+';
}


bool is_walkable(const vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
    switch (charmap[y][x])
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


bool is_switch(const vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
    switch (charmap[y][x])
    {
    case 'r':
    case 'y':
    case 'g':
    case 'b':
        return true;
    default:
        return false;
    }
}


bool can_flood(const vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
    switch (charmap[y][x])
    {
    case ' ':
        return true;
    default:
        return false;
    }
}


bool can_hold_box(const vector<vector<char> >& charmap, uint8_t x, uint8_t y)
{
    switch (charmap[y][x])
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


} // namespace boxedin
