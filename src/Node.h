#ifndef BOXED_IN_NODE
#define BOXED_IN_NODE

#include <stdio.h>
#include <string.h>
#include "boxedintypes.h"
#include "EncodedPath.h"
#include "Heuristic.h"
#include "Level.h"

// Use memory pool for Node allocation?
#define USE_NODE_MEMORY_POOL 1

#if USE_NODE_MEMORY_POOL
#include <boost/pool/pool.hpp>
#define MEMORY_POOL_NCHUNKS_START_SIZE 4096
#endif

using namespace std;

namespace boxedin
{

struct Heuristic; // forward


// TODO: Needs documentation. I don't remember what this was for.
struct BoxDescriptorLite
{
    static const int size = 3;
    uint64_t bitfields[size];
    static const int kBitfieldWidth = (sizeof(uint64_t) * 8);

    BoxDescriptorLite(const vector<Coord>& box_coords, size_t floor_width, size_t floor_height)
    {
        for (size_t i = 0; i < size; i++)
        {
            bitfields[i] = 0;
        }
        
        vector<Coord>::const_iterator it;
        for (it = box_coords.begin(); it != box_coords.end(); ++it)
        {
            set_box_bit( floor_width, *it );
        }
    }

    void set_box_bit( int floor_width, const Coord& coord )
    {
        int tile_index = get_tile_index( floor_width, coord );
        int bitfield_index = get_bitfield_index( tile_index );
        int bit_offset = get_bit_offset( tile_index );
#if 0
        fprintf(stderr, "box: set bit for x=%u y=%u tile_index=%d bitfield_index=%d bit_offset=%d\n",
                coord.x, coord.y, tile_index, bitfield_index, bit_offset);//TODO: remove
#endif
        bitfields[bitfield_index] |= ((uint64_t)1 << bit_offset);
    }
    
    void clear_box_bit( int floor_width, const Coord& coord )
    {
        int tile_index = get_tile_index( floor_width, coord );
        int bitfield_index = get_bitfield_index( tile_index );
        int bit_offset = get_bit_offset( tile_index );
        bitfields[bitfield_index] &= ~((uint64_t)1 << bit_offset);
    }
    
    bool HasBoxAt( int floor_width, const Coord& coord )
    {
        int tile_index = get_tile_index( floor_width, coord );
        int bitfield_index = get_bitfield_index( tile_index );
        int bit_offset = get_bit_offset( tile_index );
        return bitfields[bitfield_index] & ((uint64_t)1 << bit_offset);
    }
  
    int get_tile_index( int floor_width, const Coord& coord )
    {
        return (int)coord.y * floor_width + coord.x;
    }
    
    int get_bitfield_index( int tile_index )
    {
        return tile_index / kBitfieldWidth;
    }

    int get_bit_offset( int tile_index )
    {
        return tile_index % kBitfieldWidth;
    }

    void MoveUp( int floor_width, const Coord& box_coord )
    {
        clear_box_bit( floor_width, box_coord );
        Coord new_box_coord( box_coord.x, box_coord.y-1 );
        set_box_bit( floor_width, new_box_coord );
    }
    
    void MoveDown( int floor_width, const Coord& box_coord )
    {
        clear_box_bit( floor_width, box_coord );
        Coord new_box_coord( box_coord.x, box_coord.y+1 );
        set_box_bit( floor_width, new_box_coord );
    }
    
    void MoveLeft( int floor_width, const Coord& box_coord )
    {
        clear_box_bit( floor_width, box_coord );
        Coord new_box_coord( box_coord.x-1, box_coord.y );
        set_box_bit( floor_width, new_box_coord );
    }
    
    void MoveRight( int floor_width, const Coord& box_coord )
    {
        clear_box_bit( floor_width, box_coord );
        Coord new_box_coord( box_coord.x+1, box_coord.y );
        set_box_bit( floor_width, new_box_coord );
    }
};


struct GearDescriptorLite
{
    uint16_t bitfield;

    GearDescriptorLite()
        : bitfield(0)
    {
    }
    
    GearDescriptorLite(const vector<Coord>& gear_coords)
        : bitfield(0)
    {
        for (size_t i = 0; i < gear_coords.size(); i++)
        {
            bitfield |= ((uint16_t)1 << i);
        }
    }

    void ClearGearBit( const vector<Coord>& gear_coords, const Coord& clear_coord )
    {
        int sz = (int)gear_coords.size();
        for (int i = 0; i < sz; i++)
        {
            if (gear_coords[i] == clear_coord)
            {
                bitfield &= ~(1 << i);
                break;
            }
        }
    }
};


struct Action
{
    EncodedPath path;
    Coord point;
    Action(EncodedPath& path, Coord& point)
        : path(path)
        , point(point)
    {
    }
    Action(const Action& action)
        : path(action.path)
        , point(action.point)
    {
    }
    bool operator<(const Action& other) const
    {
        if (path < other.path)
          return true;
        if (other.path < path)
          return false;
        if (point.x != other.point.x)
          return point.x < other.point.x;
        if (point.y != other.point.y)
          return point.y < other.point.y;
        return false;
    }
    bool operator==(const Action& other) const
    {
        if (!(path == other.path))
            return false;
        if (!(point == other.point))
            return false;
        return true;
    }
    static Action MakeAction(std::string pathStr, Coord point)
    {
        EncodedPath encodedPath(pathStr);
        return Action(encodedPath, point);
    }
};


class Node
{
public:
    // Pointer to the Node that spawned this Node; NULL for the beginning Node
    Node* predecessor_;

    EncodedPath path_;

    /** \name Fields that uniquely identify the Node */
/**@{*/
    // The current coordinate of the player
    Coordinate<uint8_t> player_coord_;

    BoxDescriptorLite box_descriptor_;
    
    GearDescriptorLite gear_descriptor_;
/**@}*/

    // This implementation of A* does not update the gscore of a Node when a
    // better gscore is found. This is because Nodes are stored in a vector
    // of list<Node*> where each vector index is the fscore (gscore+hscore).
    // To update the gscore of the Node would also require that the Node* be
    // removed from its list<Node*> and inserted into another (unnecessarily
    // expensive). Instead, when a better gscore is found, this flag is set
    // and a new Node* is added to the appropriate list<Node*> in the vector.
    // Later, when A* removes this node from openset_fscore_nodes, this flag
    // is checked and the Node is destroyed.
    bool better_gscore_found_;

    cost_t gscore_; // cost from start to this node
    cost_t hscore_; // estimated cost form this node to goal

    Node(const Level& level, Heuristic& heuristic);

    Node(const Level& level, Heuristic& heuristic, Node& node, const Action& action);

    cost_t fscore() const { return gscore_ + hscore_; }
    
    static Node* MakeStartNode(const Level& level, Heuristic& heuristic)
    {
        Node* start = new Node(level, heuristic);
        return start;
    }

    bool IsGoal(const Level& level)
    {
        return (gear_descriptor_.bitfield == 0) && (player_coord_ == level.exit_coord_);
    }

#ifdef USE_NODE_MEMORY_POOL
    void* operator new(size_t sz);

    void operator delete(void* p);
#endif
};



bool operator<(const BoxDescriptorLite& l, const BoxDescriptorLite& r);
bool operator==(const BoxDescriptorLite& l, const BoxDescriptorLite& r);
bool operator<(const GearDescriptorLite& l, const GearDescriptorLite& r);


struct NodeCompare
{
    bool operator()(const Node* l, const Node* r) const
    {
        if (l->player_coord_ == r->player_coord_)
        {
            if (l->box_descriptor_ == r->box_descriptor_)
            {
                return (l->gear_descriptor_ < r->gear_descriptor_);
            }
            return (l->box_descriptor_ < r->box_descriptor_);
        }
        return (l->player_coord_ < r->player_coord_);
    }
};



} // namespace boxedin

#endif // BOXED_IN_NODE
