/*******************************************************************************
 * File: LevelState.cc
 *
 * Description:
 * Definitions for LevelState.
 ******************************************************************************/
#include <LevelState.h>

#include <cstring> // memcpy
#include <algorithm>

namespace boxedin {

charmap         LevelState::basemap;
Coord           LevelState::exit;
SwitchGatePairs LevelState::switchGatePairs;
Gears           LevelState::gears;


/**
   \brief Constructor
 */
LevelState::LevelState()
    : gears_bitfield(0)
{
}



/**
   \brief Copy Constructor
 */
LevelState::LevelState(const LevelState& other)
    : gears_bitfield(other.gears_bitfield)
    , boxes(other.boxes)
    , player(other.player)
{
}



/**
   \brief Create 2D character array representation of LevelState
   \details Assumes that OpenSetData is not NULL.
   \param[out] lvlmap Character array representation
   \param[in] lvlState Level state
   \param[in] drawPlayer If true, set PLAYER char at the player coordinate
 */
void LevelState::MakeLevelMap(charmap& lvlmap,
                              const LevelState& lvlState,
                              bool drawPlayer/*=true*/)
{
    const charmap& basemap = lvlState.basemap;
    const Boxes& boxes = lvlState.boxes;
    const Gears& gears = lvlState.gears;
    const Coord& p = lvlState.player;
    const SwitchGatePairs& switchGatePairs = lvlState.switchGatePairs;

    // copy space/walls/floor
    memcpy(&lvlmap, &basemap, sizeof(lvlmap));

    // set boxes
    for (BoxesCit it=boxes.begin(); it!=boxes.end(); ++it)
    {
        lvlmap[it->y][it->x] = BOX;
    }

    // set gears
    int size = (int)gears.size();
    for (int i=0; i<size; ++i)
    {
        if (lvlState.gears_bitfield & ((gears_bitfield_t)1 << i))
        {
            lvlmap[gears[i].y][gears[i].x] = GEAR;
        }
    }

    // set player
    if (drawPlayer)
    {
        lvlmap[p.y][p.x] = PLAYER;
    }

    // set switches and gates
    for (SwitchGatePairsCit it=switchGatePairs.begin();
         it!=switchGatePairs.end(); ++it)
    {
        Color c = it->first;
        Coord sw = it->second.first;
        Coord gate = it->second.second;
        if (lvlmap[sw.y][sw.x] == FLOOR)
        {
            // switch not active; draw switch and gate
            lvlmap[sw.y  ][sw.x  ] = COLOR_TO_SWITCH_CHAR(c);
            lvlmap[gate.y][gate.x] = COLOR_TO_GATE_CHAR(c);
        }               
    }
}



/**
   \brief Remove the gear at the current player coordinate.
 */
void LevelState::PickupGear()
{
    int size = (int)gears.size();
    for (int i=0; i<size; ++i)
    {
        if (gears[i] == player)
        {
            gears_bitfield &= ~((gears_bitfield_t)1 << i);
            break;
        }
    }
}



/**
   \brief Move player up, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void LevelState::MoveUp()
{
    // Move the player up
    player.y--;
    // If there's a box, move it up
    BoxesIt it = find(boxes.begin(), boxes.end(), player);
    if (it != boxes.end())
    {
        it->y--;
    }
}



/**
   \brief Move player down, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void LevelState::MoveDown()
{
    // Move the player down
    player.y++;
    // If there's a box, move it down
    BoxesIt it = find(boxes.begin(), boxes.end(), player);
    if (it != boxes.end())
    {
        it->y++;
    }
}



/**
   \brief Move player left, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void LevelState::MoveLeft()
{
    // Move the player left
    player.x--;
    // If there's a box, move it left
    BoxesIt it = find(boxes.begin(), boxes.end(), player);
    if (it != boxes.end())
    {
        it->x--;
    }
}



/**
   \brief Move player right, pushing a box if there is one.
   \details Assumes the move is legal; no validation is performed.
 */
void LevelState::MoveRight()
{
    // Move the player right
    player.x++;
    // If there's a box, move it right
    BoxesIt it = find(boxes.begin(), boxes.end(), player);
    if (it != boxes.end())
    {
        it->x++;
    }
}

} // namespace
