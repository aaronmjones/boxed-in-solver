/**
 * \file BoxedInNode.cc
 * \brief Definitions for BoxedInNode.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#include <BoxedInNode.h>

#include <boxedindefs.h>
#include <boxedintypes.h>

using namespace std;

namespace boxedin {


/**
 *   \brief Copy Constructor
 *   \param[in] other BoxedInNode to copy
 */
BoxedInNode::BoxedInNode(const BoxedInNode& other)
    : osd(NULL)
{
    *this = other;
}



/**
   \brief Constructor
   \details Construct BoxedInNode from a LevelState.
   \param[in] lvlstate LevelState to construct BoxedInNode from
 */
BoxedInNode::BoxedInNode(const LevelState& lvlstate)
    : parent(NULL)
    , osd(NULL)
{
    osd = new OpenSetData();
    osd->lvlState = lvlstate;
    osd->discard = false;
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Destructor
 */
BoxedInNode::~BoxedInNode()
{
    if (osd)
    {
        delete osd;
        osd = NULL;
    }
}



/**
   \brief BoxedInNode assignment
   \param[in] other Assign other to this.
   \returns reference to this BoxedInNode
 */
const BoxedInNode& BoxedInNode::operator=(const BoxedInNode& other)
{
    // No self assigment
    if (this == &other)
    {
        return *this;
    }

    if (osd)
    {
        delete osd;
    }

    osd = new OpenSetData(*other.osd);
    parent = other.parent;
    path = other.path;
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
    return *this;
}



/**
   \brief Sets the BoxedInNode key as a hash of the level char map
   \param[in] lvlmap Map of gears, boxes, walls, etc to generate key from
 */
void BoxedInNode::SetKey(charmap& lvlmap)
{
    string hash_string(&lvlmap[0][0], WIDTH*HEIGHT);
    key = hash<string>()(hash_string);
}



/**
   \brief Pick up the gear located at the ActionPoint coordinate
   \param[in] actionPoint Coordinate of gear and path from parent BoxedInNode
              to it.
 */
void BoxedInNode::ActionPointGear(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Remove gear
    lvlState.PickupGear();
    // Update path for moves just made
    path = actionPoint.path;

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Move player position to the exit located at the ActionPoint
          coordinate.
   \param actionPoint Coordinate of exit and path from parent to it.
 */
void BoxedInNode::ActionPointExit(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Update path for moves just made
    path = actionPoint.path;

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Move box located at ActionPoint coordinate, up.
   \param actionPoint Coordinate underneath box and path from parent to it.
 */
void BoxedInNode::ActionPointUp(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Move up
    lvlState.MoveUp();
    // Update path for the moves just made
    path = actionPoint.path;
    path.push_back('U');

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Move box located at ActionPoint coordinate, down.
   \param actionPoint Coordinate above box and path from parent to it.
 */
void BoxedInNode::ActionPointDown(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Move down
    lvlState.MoveDown();
    // Update path for the moves just made
    path = actionPoint.path;
    path.push_back('D');

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Move box located at ActionPoint coordinate, left.
   \param actionPoint Coordinate right of box and path from parent to it.
 */
void BoxedInNode::ActionPointLeft(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Move left
    lvlState.MoveLeft();
    // Update path for the moves just made
    path = actionPoint.path;
    path.push_back('L');

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Move box located at ActionPoint coordinate, right.
   \param actionPoint Coordinate left of box and path from parent to it.
 */
void BoxedInNode::ActionPointRight(ActionPoint& actionPoint)
{
    LevelState& lvlState = osd->lvlState;

    // Jump to action point
    lvlState.player = actionPoint.coord;
    // Move right
    lvlState.MoveRight();
    // Update path for the moves just made
    path = actionPoint.path;
    path.push_back('R');

    // Update the key for this new level state
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, osd->lvlState);
    SetKey(lvlmap);
}



/**
   \brief Constructor
 */
OpenSetData::OpenSetData()
    : f(0)
    , g(0)
    , bitfield(0)
{
}



/**
   \brief Copy Constructor
 */
OpenSetData::OpenSetData(const OpenSetData& other)
    : lvlState(other.lvlState)
    , f(other.f)
    , g(other.g)
    , bitfield(other.bitfield)
{
}


} // namespace
