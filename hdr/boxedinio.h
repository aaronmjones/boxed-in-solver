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
#include "SearchResult.h"

namespace boxedin {
namespace io {

bool ParseSolution(std::istream& in, Path& path);
void ParseCharMap(std::istream& in, std::vector<std::vector<char> >& charmap);
bool IsValidBoxedInLevel(std::vector<std::vector<char> >& charmap);

} // namespace
} // namespace


std::ostream& clear_screen(std::ostream& out);

void PrintCharMap(std::ostream& out, std::vector<std::vector<char> >& charmap, bool use_color = true);

/** \relates Path */
std::ostream& operator<<(std::ostream& out, const boxedin::Path& path);
/** \relates Coord */
std::ostream& operator<<(std::ostream& out, const boxedin::Coord& coord);
std::ostream& operator<<(std::ostream& out, const std::list<boxedin::Path>& solution);
std::ostream& operator<<(std::ostream& out, const boxedin::SearchResult& result);


#endif
