/**
 * \file LevelState.h
 * \brief This file contains the LevelState declaration.
 * \details The LevelState class completely characterizes the state of a Boxed
 *          In level.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#ifndef LEVEL_STATE_H__
#define LEVEL_STATE_H__
#include "boxedindefs.h"
#include "boxedintypes.h"


namespace boxedin {

/**
   \class LevelState
   \brief Characterizes the state of a Boxed In level.
 */
class LevelState
{
public:
    /** The space, walls and floor */
    static charmap basemap;

    /** The exit position */
    static Coord exit;

    /** Pairs of switch/gate coordinates */
    static SwitchGatePairs switchGatePairs;

    /** Gear locations */
    static Gears gears;

    /** This bitfield tells whether each gear has been picked up */
    gears_bitfield_t gears_bitfield;

    /** Box positions */
    Boxes boxes;

    /** Player position */
    Coord player;

    /** Constructor */
    LevelState();

    /** Copy constructor */
    LevelState(const LevelState& other);

    /** Make character map representation from LevelState */
    static void MakeLevelMap(charmap& lvlmap, const LevelState& lvlState,
                             bool drawPlayer=true);

    /** "Pickup" the gear at the player's current position */
    void PickupGear();

    /** Move player up and move box accordingly */
    void MoveUp();

    /** Move player down and move box accordingly */
    void MoveDown();

    /** Move player left and move box accordingly */
    void MoveLeft();

    /** Move player right and move box accordingly */
    void MoveRight();
};


} // namespace

#endif
