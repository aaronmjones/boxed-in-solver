/**
 * \file boxedinio.h
 * \brief This file containts declarations for I/O functions.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#ifndef BOXED_IN_IO_H__
#define BOXED_IN_IO_H__

#include <list>
#include <vector>
#include <istream>
#include <ostream>

#include "boxedindefs.h"
#include "boxedintypes.h"
#include "BoxedInNode.h"
#include "LevelState.h"
#include "SearchResult.h"

namespace boxedin {
/**
 \namespace boxedin::io
 \brief Classes and static functions use for I/O.

 \section levelfile_sec Level File

 \subsection levelfilechars_sec Level File Characters
 \brief A level file is composed of the following characters
 \verbatim
 ' space
 x wall
   floor (space character)
 + box
 * gear
 r red switch
 g green switch
 b blue switch
 y yellow switch
 R red gate
 G green gate
 B blue gate
 Y yellow gate
 @ exit
  \endverbatim

  \subsection levelfileexample_sec Level File Example
  \brief Game 1, Level 37 (levels/01/37.txt):
  \verbatim
''''''''''
xxxxxxxxxx
xp   +   x
xr Y + x x
x  x*x   x
x   +xx yx
xxx++ x x'
x +     x'
'x xx x x'
'x      x'
'xRxxxxGx'
'xg*x'x@x'
'xxxx'xxx'
''''''''''
  \endverbatim
 */
namespace io {

/** This is a flag indicating whether to print Boxed In levels in color. */
extern bool use_colors;

bool ParseLevel(std::istream& in, LevelState& levelState);
bool ParseSolution(std::istream& in, Path& path);
void ParseCharMap(std::istream& in, std::vector<std::vector<char> >& charmap);
bool IsValidBoxedInLevel(std::vector<std::vector<char> >& charmap);

} // namespace
} // namespace


std::ostream& clear_screen(std::ostream& out);

void PrintCharMapInColor(std::ostream& out, std::vector<std::vector<char> >& charmap);

std::ostream& operator<<(std::ostream& out, const boxedin::charmap& lvlmap);
/** \relates Path */
std::ostream& operator<<(std::ostream& out, const boxedin::Path& path);
/** \relates Coord */
std::ostream& operator<<(std::ostream& out, const boxedin::Coord& coord);
/**
   \relates boxedin::BoxedInNode
   Print a BoxedInNode.
 */
std::ostream& operator<<(std::ostream& out, const boxedin::BoxedInNode* goal);
std::ostream& operator<<(std::ostream& out, const boxedin::BoxedInNode& node);
std::ostream& operator<<(std::ostream& out, const boxedin::ActionPoints& actionPoints);
std::ostream& operator<<(std::ostream& out, const std::list<boxedin::Path>& solution);
std::ostream& operator<<(std::ostream& out, const boxedin::SearchResult& result);


#endif
