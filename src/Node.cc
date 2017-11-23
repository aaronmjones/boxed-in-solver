#include "Heuristic.h"
#include "Node.h"

using namespace boost;

namespace boxedin
{

#if USE_NODE_MEMORY_POOL
pool<default_user_allocator_new_delete> memory_pool(sizeof(Node), MEMORY_POOL_NCHUNKS_START_SIZE);
#endif

Node::Node(const Level& level, Heuristic& heuristic)
    : predecessor_(NULL)
    , player_coord_(level.player_coord_)
    , box_descriptor_(level.box_coords_,
                      level.floor_plan_[0].size(),
                      level.floor_plan_.size())
    , gear_descriptor_(level.gear_coords_)
    , better_gscore_found_(false)
    , gscore_(0)
    , hscore_((cost_t)0)
{
    hscore_ = heuristic.get_hscore(*this);
}

Node::Node(const Level& level, Heuristic& heuristic, Node& node, const Action& action)
    : predecessor_(&node)
    , path_(action.path)
    , player_coord_(action.point)
    , box_descriptor_(node.box_descriptor_)
    , gear_descriptor_(node.gear_descriptor_)
    , better_gscore_found_(false)
    , gscore_(node.gscore_ + action.path.size())
    , hscore_((cost_t)0)
{
    // If there is a gear at the action point, this will clear it.
    gear_descriptor_.ClearGearBit( level.gear_coords_, action.point );

    if (box_descriptor_.HasBoxAt( (int)level.floor_plan_[0].size(), action.point ) )
    {
      EncodedPathDirection direction = action.path.at(action.path.size() - 1);
      switch (direction)
      {
      case ENCODED_PATH_DIRECTION_UP:
        box_descriptor_.MoveUp( (int)level.floor_plan_[0].size(), action.point );
        break;
      case ENCODED_PATH_DIRECTION_DOWN:
        box_descriptor_.MoveDown( (int)level.floor_plan_[0].size(), action.point );
        break;
      case ENCODED_PATH_DIRECTION_LEFT:
        box_descriptor_.MoveLeft( (int)level.floor_plan_[0].size(), action.point );
        break;
      case ENCODED_PATH_DIRECTION_RIGHT:
        box_descriptor_.MoveRight( (int)level.floor_plan_[0].size(), action.point );
        break;
      }
    }
        
    hscore_ = heuristic.get_hscore(*this);
}

#ifdef USE_NODE_MEMORY_POOL
void* Node::operator new(size_t sz)
{
    return memory_pool.malloc();
}

void Node::operator delete(void* p)
{
    memory_pool.free(p);
}
#endif

bool operator<(const BoxDescriptorLite& l, const BoxDescriptorLite& r)
{
    if (l.size == r.size)
    {
        for (size_t i = 0; i < l.size; i++)
        {
            if (l.bitfields[i] == r.bitfields[i])
            {
                continue;
            }
            return (l.bitfields[i] < r.bitfields[i]);
        }
        return false; // box descriptor bitfields have equal size and values
    }
    return (l.size < r.size);
}

bool operator==(const BoxDescriptorLite& l, const BoxDescriptorLite& r)
{
    if (l.size == r.size)
    {
        for (size_t i = 0; i < l.size; i++)
        {
            if (l.bitfields[i] == r.bitfields[i])
            {
                continue;
            }
            return false;
        }
        return true;
    }
    return false;
}

bool operator<(const GearDescriptorLite& l, const GearDescriptorLite& r)
{
    return (l.bitfield < r.bitfield);
}


} // namespace boxedin
