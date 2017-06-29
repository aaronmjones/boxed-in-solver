#include "Node.h"

namespace boxedin
{


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
