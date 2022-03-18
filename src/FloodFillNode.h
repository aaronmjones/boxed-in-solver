#ifndef FLOOD_FILL_NODE_H__
#define FLOOD_FILL_NODE_H__

#include "EncodedPath.h"

namespace boxedin
{

/**
   \struct FloodFillNode
   \brief This type is used for flood filling a level state to find action
          points
 */
struct FloodFillNode
{
    Coord coord;
    EncodedPath path;
    FloodFillNode() {}
    FloodFillNode(const FloodFillNode& other)
        : coord(other.coord), path(other.path) {}
    FloodFillNode(const Coord& coord)
        : coord(coord) {}
};

} // namespace boxedin

#endif
