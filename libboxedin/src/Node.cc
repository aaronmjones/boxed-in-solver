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
    , hscore_((cost_t)-1)
{
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
