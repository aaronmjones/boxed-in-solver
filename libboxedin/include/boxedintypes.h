/**
 * \file boxedintypes.h
 * \brief This file contains various utility types used for the Boxed In solver
 *        A* algorithm.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#ifndef BOXED_IN_TYPES_H__
#define BOXED_IN_TYPES_H__

#include "boxedindefs.h"

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include <list>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>


namespace boxedin {


// forward declarations
class BoxedInNode;


/**
   \enum Color
   \brief Color values for switches and gates
 */
enum Color
{
    RED,
    YELLOW,
    GREEN,
    BLUE
};



/**
   \struct Coordinate
   \brief Template for an x,y coordinate
   \tparam T The type of x and y
 */
template<typename T>
struct Coordinate
{
    T x;
    T y;
    Coordinate() { x=0; y=0; }
    Coordinate(T _x, T _y) { x=_x; y=_y; }
    Coordinate(const Coordinate& other) { x=other.x; y=other.y; }
    bool operator==(const Coordinate& other) const {
        return (x==other.x && y==other.y);
    }
};

template<typename T>
    bool operator<(const Coordinate<T>& l, const Coordinate<T>& r)
{
    if (l.y == r.y)
    {
        return (l.x < r.x);
    }
    return (l.y < r.y);
}

template<typename T>
    bool operator==(const Coordinate<T>& l, const Coordinate<T>& r)
{
    return ((l.y == r.y) && (l.x == r.x));
}

/** \group gears_bitfield_t
    The position of each gear is stored in 1 static container
    and is referenced by every LevelState. Every LevelState has
    its own bitfield which describes wheter each gear has been
    picked up or not.
    Each bit index corresponds to a gear in the gears container
    with the same index.
    If the bit value is 1, the gear has not been picked up.
    If the bit value is 0, the gear has been picked up.
 */
/**@{*/
#if GEARS_MAX == GEARS_8
typedef uint8_t gears_bitfield_t;
#elif GEARS_MAX == GEARS_16
typedef uint16_t gears_bitfield_t;
#elif GEARS_MAX == GEARS_32
typedef uint32_t gears_bitfield_t;
#else
#error "GEARS_MAX is undefined or invalid"
#endif
/**@}*/



/** A 2D char array the size of a Boxed In level. */
typedef char charmap[HEIGHT][WIDTH];

/** An x/y coordinate. */
typedef Coordinate<uint8_t> Coord;

/** A pair of switch/gate coordinates. */
typedef std::pair<Coord,Coord> SwitchGatePair;

/** Maps each color to a switch/gate coordinate pair. */
typedef std::map<Color,SwitchGatePair> SwitchGatePairs;
/** SwitchGatePairs iterator. */
typedef SwitchGatePairs::iterator SwitchGatePairsIt;
/** SwitchGatePairs const iterator. */
typedef SwitchGatePairs::const_iterator SwitchGatePairsCit;

/** Generic coordiate vector. */
typedef std::vector<Coord> Coords;

/** Vector of box coordinates. */
typedef std::vector<Coord> Boxes;
/** Boxes iterator. */
typedef Boxes::iterator BoxesIt;
/** Boxes const iterator. */
typedef Boxes::const_iterator BoxesCit;

/** Vector of gear coordinates. */
typedef std::vector<Coord> Gears;
/** Gears iterator. */
typedef Gears::iterator GearsIt;
/** Gears const iterator. */
typedef Gears::const_iterator GearsCit;

/** Path of direction chars {u,d,l,r}. */
typedef std::vector<char> Path;
/** Path iterator. */
typedef Path::iterator PathIt;
/** Path const iterator. */
typedef Path::const_iterator PathCit;

/** The type used for a key for open set and closed set containers. */
typedef size_t key_type;

/** The type used for f, g and h values. */
typedef int cost_t;

#define COST_INFINITY (INT_MAX-0)
#define COST_UNKNOWN  (INT_MAX-1)

/**
   \struct ActionPoint
   \brief This is used to generate successors to a BoxedInNode.
   \details The algorithm expands each BoxedInNode to generate a list of
            BoxedInNode's that can be reached by doing some action. Possible
            actions are: moving a box, stepping on a gear or stepping on the
	    exit.
	    An ActionPoint's coord is one of:
	    \li coordinate where the player can push an adjacent box,
	    \li coordinate of a gear
	    \li coordinate of the exit

	    An ActoinPoint's path is the path from the BoxedInNode's player
	    coordinate to the ActionPoint coordinate and is used to create the
	    successor BoxedInNode's path.
 */
struct ActionPoint
{
    Path path;
    Coord coord;
    ActionPoint() {}
    ActionPoint(const ActionPoint& other)
        : path(other.path), coord(other.coord) {}
    ActionPoint(const Path& path, const Coord& coord)
        : path(path), coord(coord) {}
};



/**
   \struct FloodFillNode
   \brief This type is used for flood filling a level state to find action
          points
 */
struct FloodFillNode
{
    Coord coord;
    Path path;
    FloodFillNode() {}
    FloodFillNode(const FloodFillNode& other)
        : coord(other.coord), path(other.path) {}
    FloodFillNode(const Coord& coord)
        : coord(coord) {}
};



/** This container is used to find action points */
typedef std::queue<FloodFillNode> FloodFillQueue;
/** Vector of points where something interesting can happen, like pickup a gear,
    step on an exit or move a box. */
typedef std::vector<ActionPoint> ActionPoints;
/** ActionPoints iterator. */
typedef ActionPoints::iterator ActionPointIt;
/** ActoinPoints const iterator. */
typedef ActionPoints::const_iterator ActionPointCit;



/** List of BoxedInNode created by expanding parent node (apply ActionPoints) */
typedef std::list<BoxedInNode*> BoxedInNodeList;
/** BoxedInNodeList iterator. */
typedef BoxedInNodeList::iterator BoxedInNodeListIt;
/** BoxedInNodelist const iterator. */
typedef BoxedInNodeList::const_iterator BoxedInNodeListCit;



} // namespace

#endif
