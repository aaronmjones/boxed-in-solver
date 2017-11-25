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
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace boxedin;

namespace boxedin {
namespace io {


void ParseCharMap(istream& in, vector<vector<char> >& charmap)
{
    char c;
    vector<char> row;
    while (in.get(c))
    {
        switch (c)
        {
        case '\n':
            charmap.push_back(row);
            row.clear();
            break;;
        default:
            row.push_back(c);
            break;
        }
    }
    if (!row.empty())
    {
        charmap.push_back(row);
    }
}

bool IsValidBoxedInLevel(vector<vector<char> >& charmap)
{
    int player_chars_found = 0;
    int exit_chars_found = 0;
    int invalid_chars_found = 0;
    for (size_t y = 0; y < charmap.size(); y++)
    {
        if (y > 0 && charmap[y].size() != charmap[y-1].size())
        {
            return false;
        }
        for (size_t x = 0; x < charmap[y].size(); x++)
        {
            switch (charmap[y][x])
            {
            case '\'': // space
            case ' ':  // floor
            case 'x':  // wall
            case '+':  // box
            case '*':  // gear
            case 'r':  // switch
            case 'o':  // switch
            case 'y':  // switch
            case 'g':  // switch
            case 'b':  // switch
            case 'v':  // switch
            case 'R':  // gate
            case 'O':  // gate
            case 'Y':  // gate
            case 'G':  // gate
            case 'B':  // gate
            case 'V':  // gate
                break;
            case 'p':  // player
                player_chars_found++;
                break;
            case '@':  // exit
                exit_chars_found++;
                break;
            default:
                fprintf(stderr, "Invalid char '%c' found in level file\n", charmap[y][x]);
                invalid_chars_found++;
                break;
            }
        }
    }
    if (player_chars_found != 1)
    {
        fprintf(stderr, "Invalid level file: %d player chars found\n", player_chars_found);
    }
    if (exit_chars_found != 1)
    {
        fprintf(stderr, "Invalid level file: %d exit chars found\n", exit_chars_found);
    }
    return ( player_chars_found == 1 && exit_chars_found == 1 &&
             invalid_chars_found == 0);
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
const char* yellow  = "\x1b[1;33m";
const char* green   = "\x1b[1;32m";
const char* blue    = "\x1b[1;34m";
const char* magenta = "\x1b[1;35m";
const char* cyan    = "\x1b[1;36m";
const char* brown   = "\x1b[0;33m";
const char* white   = "\x1b[1;37m";
#elif defined(_WIN32)
#include <windows.h>
inline std::ostream& normal(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return s;
}

inline std::ostream& red(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
			    FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& yellow(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& green(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
			    FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& blue(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
			    | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& brown(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
			    FOREGROUND_GREEN | FOREGROUND_RED);
    return s;
}

inline std::ostream& cyan(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& magenta(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
        FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& white(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout,
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    return s;
}

struct color {
    color(WORD attribute) : m_color(attribute) {};
    WORD m_color;
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>&
operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, c.m_color);
    return i;
}
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



void PrintCharMap(ostream& out, vector<vector<char> >& charmap, bool use_color/*=true*/)
{
  // Display level, optionally with color
  for (size_t y = 0; y < charmap.size(); y++)
  {
    for (size_t x = 0; x < charmap[y].size(); x++)
    {
      if (use_color)
      {
        switch (charmap[y][x])
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
        case SWITCH_ORANGE:
        case GATE_ORANGE:
          out << yellow;  break; // I don't know how to display orange text, so reuse yellow
        case SWITCH_YELLOW:
        case GATE_YELLOW:
          out << yellow;  break;
        case SWITCH_GREEN:
        case GATE_GREEN:
          out << green;   break;
        case SWITCH_BLUE:
        case GATE_BLUE:
          out << blue;    break;
        case SWITCH_VIOLET:
        case GATE_VIOLET:
          out << magenta; break;
        default:
          out << normal;  break;
        }
      }
      out << charmap[y][x];
    }
    out << endl;
  }
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

