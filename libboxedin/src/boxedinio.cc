/**
 * \file boxedinio.cc
 * \brief Definitions for IO functions.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */
#include <boxedinio.h>

#include <cstring>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace boxedin;

namespace boxedin {
    namespace io {


        bool use_colors = false;


        /**
           \brief Parse Boxed In level from ASCII char, file representation.
           \param[in] in Input stream to parse level from.
           \param[out] lvlState LevelState created from parsed input.
           \returns true if successful, or false if an error occured.

	   \sa \ref levelfile_sec
         */
        bool ParseLevel(istream& in, LevelState& lvlState)
        {
            Color color;
            char c;
            int i = 0;
            int j = 0;
            while (in.get(c))
            {
                switch (c)
                {
                case '\n':
                    continue;
                case SWITCH_RED:
                case SWITCH_GREEN:
                case SWITCH_BLUE:
                case SWITCH_YELLOW:
                    color = SWITCH_CHAR_TO_COLOR(c);
                    lvlState.switchGatePairs[color].first = Coord(i, j);
                    lvlState.basemap[j][i] = FLOOR;
                    break;
                case GATE_RED:
                case GATE_GREEN:
                case GATE_BLUE:
                case GATE_YELLOW:
                    color = GATE_CHAR_TO_COLOR(c);
                    lvlState.switchGatePairs[color].second = Coord(i, j);
                    lvlState.basemap[j][i] = FLOOR;
                    break;
                case PLAYER:
                    lvlState.player = Coord(i, j);
                    lvlState.basemap[j][i] = FLOOR;
                    break;
                case GEAR:
                    lvlState.gears_bitfield |=
                        ((gears_bitfield_t)1 << lvlState.gears.size());
                    lvlState.gears.push_back(Coord(i, j));
                    lvlState.basemap[j][i] = FLOOR;
                    break;
                case BOX:
                    lvlState.boxes.push_back(Coord(i, j));
                    lvlState.basemap[j][i] = FLOOR;
                    break;
                case EXIT:
                    lvlState.exit = Coord(i, j);
                    lvlState.basemap[j][i] = EXIT;
                    break;
                default:
                    lvlState.basemap[j][i] = c;
                    break;
                }

                if (++i >= WIDTH)
                {
                    i = 0;
                    if (++j >= HEIGHT)
                    {
                        break;
                    }
                }
            }
            return true;
        }

        // The path file contains directions to solve the level.
        //
        // The directions can be delimited by ' ', ',', ':', ';', '\n'
        //
        // The directions can be any of:
        // u,d,l,r
        // U,D,L,R
        // up,down,left,right
        // UP,DOWN,LEFT,RIGHT
        // where the delimiter can be any of the delimiters listed above.
        bool ParseSolution(istream& in, Path& path)
        {
            char c = 0;
            while (in.get(c))
            {
                c = toupper(c);
                switch (c)
                {
                case 'U':
                case 'D':
                case 'L':
                case 'R':
                    path.push_back(c);
                    break;
                default:
                    break;
                }
            }

            return true;
        }


    } // namespace
} // namespace

#if defined(__linux__) || defined(__APPLE__)
const char* normal  = "\x1b[0;39m";
const char* red     = "\x1b[1;31m";
const char* green   = "\x1b[1;32m";
const char* yellow  = "\x1b[1;33m";
const char* blue    = "\x1b[1;34m";
const char* magenta = "\x1b[1;35m";
const char* cyan    = "\x1b[1;36m";
const char* brown   = "\x1b[0;33m";
const char* white   = "\x1b[1;37m";
#elif defined(_WIN32)
#include <ConsoleColor.h>
#endif


ostream& clear_screen(ostream& out)
{
#if defined(__linux__) || defined(__APPLE__)
    out << "\033c";
#elif defined(_WIN32)
    system("cls");
#endif
    return out;
}



/**
   \relates boxedin::charmap
   \brief Print the map of level characters.
   \details Certain characters are printed in color if use_color is true.
   \param[in,out] out ostream to print to.
   \param[in] lvlmap Character map to print.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const charmap& lvlmap)
{
    // Display level, optionally with color
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            char c = lvlmap[y][x];
            if (boxedin::io::use_colors)
            {
                switch (c)
                {
                case BOX:
                    out << brown;   break;
                case PLAYER:
                    out << cyan;    break;
                case EXIT:
                    out << magenta; break;
                case GEAR:
                    out << white;   break;
                case SWITCH_RED:
                case GATE_RED:
                    out << red;     break;
                case SWITCH_YELLOW:
                case GATE_YELLOW:
                    out << yellow;  break;
                case SWITCH_GREEN:
                case GATE_GREEN:
                    out << green;   break;
                case SWITCH_BLUE:
                case GATE_BLUE:
                    out << blue;    break;
                default:
                    out << normal;  break;
                }
            }
            out << c;
        }
        out << endl;
    }
    return out;
}



/**
   \relates boxedin::Path
   \brief Print the path characters with no delimiters.
   \param[in,out] out ostream to print to.
   \param[in] path Characters to print.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const Path& path)
{
    for (PathCit it = path.begin(); it != path.end(); ++it)
    {
        out << *it;
    }
    return out;
}



/**
   \relates boxedin::Coord
   \brief Print the coordinate.
   \param[in,out] out ostream to print to.
   \param[in] coord Coordinate to print.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const Coord& coord)
{
    out << "(" << (int)coord.x << "," << (int)coord.y << ")";
    return out;
}



/**
   \relates boxedin::BoxedInNode
   \brief Print the paths from goal to start node.
   \param[in,out] out ostream to print to.
   \param[in] goal The BoxedInNode goal.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const BoxedInNode* goal)
{
    const BoxedInNode* node = goal;
    while (node)
    {
        out << node->path << endl;
        node = node->parent;
    }
    return out;
}



/**
   \relates boxedin::BoxedInNode
   \brief Creates a level map from node and displays it.
   \details Certain characters are printed in color if use_color is true.
   \param[in,out] out ostream to print to.
   \param[in] node The node to display.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const BoxedInNode& node)
{
    charmap lvlmap;
    LevelState::MakeLevelMap(lvlmap, node.osd->lvlState);
    out << lvlmap;
    return out;
}



/**
   \relates boxedin::ActionPoint
   \brief Displays all ActionPoint coordinates.
   \param[in,out] out ostream to print to.
   \param[in] actionPoints A list of ActionPoint's to display.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const ActionPoints& actionPoints)
{
    for (ActionPointCit it = actionPoints.begin(); it != actionPoints.end(); ++it)
    {
        out << it->coord << " ";
    }
    out << endl;
    return out;
}



/**
   \relates boxedin::Path
   \brief Displays the solution chars with no delimiter.
   \param[in,out] out ostream to print to.
   \param[in] solution A list of Path's to display.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const list<Path>& solution)
{
    list<Path>::const_iterator cit = solution.begin();

    while (cit != solution.end())
    {
        const Path& path = *cit;
        out << path;
        ++cit;
    }

    return out;
}



/**
   \relates MemUsage
   \brief Displays process memory usage information.
   \param[in,out] out ostream to print to.
   \param[in] memusage Process memory usage information.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const MemUsage& memusage)
{
    out << "MAXRSS " << memusage.max_resident_set_size;
    return out;
}



/**
   \relates boxedin::SearchResult
   \brief Displays A* search result information.
   \param[in,out] out ostream to print to.
   \param[in] result A* search result.
   \returns A reference to the ostream.
 */
ostream& operator<<(ostream& out, const SearchResult& result)
{
    const steady_clock::time_point& t1 = result.search_start_time;
    const steady_clock::time_point& t2 = result.search_stop_time;

    seconds time_span = duration_cast<seconds>(t2 - t1);
    out << "SEARCHTIME " << time_span.count() << endl;

    if (result.success)
    {
        out << "Level can be solved in " << result.num_moves << " moves" << endl;
    }

    out << "A* search time was ";

    if (time_span.count() < 10)
    {
        // Print search time with high resolution
        duration<double> high_res = duration_cast<duration<double>>(t2 - t1);
        out << high_res.count() << " seconds" << endl;
    }
    else
    {
        hours hrs = duration_cast<hours>(time_span);
        time_span -= hrs;
        minutes mins = duration_cast<minutes>(time_span);
        time_span -= mins;
        seconds secs = duration_cast<seconds>(time_span);

        // Print search time with seconds resolution
        if (hrs.count())
        {
            out << hrs.count() << " hours " << mins.count() << " minutes "
                << secs.count() << " seconds" << endl;
        }
        else if (mins.count())
        {
            out << mins.count() << " minutes " << secs.count() << " seconds" << endl;
        }
        else
        {
            out << secs.count() << " seconds" << endl;
        }
    }

    out << "Nodes in open set " << result.openset_size << endl;
    out << "Nodes in closed set " << result.closedset_size << endl;

    out << result.memusage << endl;

    return out;
}

