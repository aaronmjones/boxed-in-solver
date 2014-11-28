/**
 * \file BoxedInNode.h
 * \ingroup boxedin_group
 * \brief Declarations for BoxedInNode.
 */
#ifndef BOXED_IN_NODE_H__
#define BOXED_IN_NODE_H__
#include <boxedindefs.h>
#include <boxedintypes.h>
#include <LevelState.h>


namespace boxedin {

class OpenSetData;

/**
   \class BoxedInNode
   \brief This is the search node for A* open set and closed set.
   \details Each BoxedInNode is uniquely identified by key, which is a hash
            of the character map representation of the state of the node.

	    Certain data fields are only needed while a BoxedInNode is in the
	    open set. These fields have been aggregated into the OpenSetData
	    struct and are deleted once the node is moved into the closed set
	    in order to reduce the amount memory used.
 */
class BoxedInNode
{
public:
    /** \brief Unique key for this level state obtained from hashing level map */
    key_type key;

    /** \brief The parent node "expanded" to create this node */
    const BoxedInNode* parent;

    /** \brief The U,D,L,R directions from parent to this node */
    Path path;

    /** \brief Data only needed while node is in open set */
    OpenSetData* osd;

    BoxedInNode(const BoxedInNode& other);

    BoxedInNode(const LevelState& lvlstate); 

    ~BoxedInNode();

    const BoxedInNode& operator=(const BoxedInNode& other);

    void SetKey(charmap& lvlmap);

    void ActionPointGear(ActionPoint& actionPoint);

    void ActionPointExit(ActionPoint& actionPoint);

    void ActionPointUp(ActionPoint& actionPoint);

    void ActionPointDown(ActionPoint& actionPoint);

    void ActionPointLeft(ActionPoint& actionPoint);

    void ActionPointRight(ActionPoint& actionPoint);
};



/**
   \class OpenSetData
   \brief This type contains data only needed while node is in open set.
 */
class OpenSetData
{
public:
    LevelState lvlState;
    cost_t f;
    cost_t g;
    union {
	uint8_t bitfield;
	struct {
	    uint8_t discard     : 1;
	    uint8_t blacklisted : 1;
	    uint8_t unused      : 6;
	};
    };
    OpenSetData();
    OpenSetData(const OpenSetData& other);
};


} // namespace


#endif
