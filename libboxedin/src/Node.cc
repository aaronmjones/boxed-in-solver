#include "Heuristic.h"
#include "Node.h"

namespace boxedin
{


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
    switch (action.type)
    {
    case ACTION_TYPE_PICKUP_GEAR:
        gear_descriptor_.ClearGearBit( level.gear_coords_, action.point );
        break;
    case ACTION_TYPE_EXIT:
        // Nothing to do; player coord and gscore updated above; hscore updated below
        break;
    case ACTION_TYPE_MOVE_BOX_UP:
        box_descriptor_.MoveUp( (int)level.floor_plan_[0].size(), action.point );
        break;
    case ACTION_TYPE_MOVE_BOX_DOWN:
        box_descriptor_.MoveDown( (int)level.floor_plan_[0].size(), action.point );
        break;
    case ACTION_TYPE_MOVE_BOX_LEFT:
        box_descriptor_.MoveLeft( (int)level.floor_plan_[0].size(), action.point );
        break;
    case ACTION_TYPE_MOVE_BOX_RIGHT:
        box_descriptor_.MoveRight( (int)level.floor_plan_[0].size(), action.point );
        break;
    }
    hscore_ = heuristic.get_hscore(*this);
}
    
bool operator<(const BoxDescriptorLite& l, const BoxDescriptorLite& r)
{
    if (l.bitfields.size() == r.bitfields.size())
    {
        for (size_t i = 0; i < l.bitfields.size(); i++)
        {
            if (l.bitfields[i] == r.bitfields[i])
            {
                continue;
            }
            return (l.bitfields[i] < r.bitfields[i]);
        }
        return false; // box descriptor bitfields have equal size and values
    }
    return (l.bitfields.size() < r.bitfields.size());
}

bool operator==(const BoxDescriptorLite& l, const BoxDescriptorLite& r)
{
    if (l.bitfields.size() == r.bitfields.size())
    {
        for (size_t i = 0; i < l.bitfields.size(); i++)
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
