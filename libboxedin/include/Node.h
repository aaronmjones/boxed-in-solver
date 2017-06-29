#ifndef BOXED_IN_NODE
#define BOXED_IN_NODE

#include "boxedintypes.h"
#include "Heuristic.h"
#include "Level.h"

using namespace std;

namespace boxedin
{
    
struct BoxDescriptorLite
{
    vector<uint64_t> bitfields;
    static const int kBitfieldWidth = (sizeof(uint64_t) * 8);

    BoxDescriptorLite()
    {
    }
    
    BoxDescriptorLite(const vector<Coord>& box_coords, size_t floor_rows, size_t floor_cols)
    {
        size_t num_tiles = floor_rows * floor_cols;
        size_t bitfields_size = (num_tiles + (floor_cols - 1)) / kBitfieldWidth;
        bitfields.resize( bitfields_size );
        
        vector<Coord>::const_iterator it;
        for (it = box_coords.begin(); it != box_coords.end(); ++it)
        {
            uint64_t tile_index = it->y * floor_cols + it->x;
            uint64_t bitfield_index = tile_index / kBitfieldWidth;
            uint64_t bit_offset = tile_index % kBitfieldWidth;
            bitfields[bitfield_index] |= ((uint64_t)1 << bit_offset);
        }
        for (size_t i = 0; i < bitfields.size(); i++)//TODO: remove
        {
            fprintf(stderr, "bitfields[%d]=0x%016lx\n", (int)i, bitfields[i]);
        }
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
};


class Node
{
public:
    // Pointer to the Node that spawned this Node; NULL for the beginning Node
    Node* predecessor_;

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
    Node()
        : predecessor_(NULL)
        , better_gscore_found_(false)
        , gscore_(0)
        , hscore_(0)
    {
    }

    Node(const Level& level, const Heuristic& heuristic)
        : predecessor_(NULL)
        , player_coord_(level.player_coord_)
        , box_descriptor_(level.box_coords_,
                          level.floor_plan_[0].size(),
                          level.floor_plan_.size())
        , gear_descriptor_(level.gear_coords_)
        , better_gscore_found_(false)
        , gscore_(0)
        , hscore_((cost_t)-1)
    {
        hscore_ = heuristic.calculate(level, *this);
    }

    cost_t fscore() const { return gscore_ + hscore_; }
    
    static Node* MakeStartNode(const Level& level, const Heuristic& heuristic)
    {
        Node* start = new Node(level, heuristic);
        return start;
    }
};


bool operator<(const BoxDescriptorLite& l, const BoxDescriptorLite& r);
bool operator==(const BoxDescriptorLite& l, const BoxDescriptorLite& r);
bool operator<(const GearDescriptorLite& l, const GearDescriptorLite& r);


struct NodeCompare
{
    bool operator()(const Node* l, const Node* r)
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
