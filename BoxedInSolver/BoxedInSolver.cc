/**
 * \file BoxedInSolver.cc
 * \brief This file contains the main() function for BoxedInSolver.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */

#if defined (_WIN32)
# include <wingetopt.h>
#elif defined (__linux__) || defined (__APPLE__)
# include <getopt.h>
# include <signal.h>
# include <execinfo.h> // backtrace
#endif

#include <cstdlib>
#include <cstdio>
#include <time.h>

#include <fstream>
#include <iostream>

#include <boxedinio.h>
#include <astar.h>
#include "Node.h"
#include "Level.h"
#include "Heuristic.h"


#if defined (__linux__) || defined (__APPLE__)
static void signal_handler(int signum);
#endif

static void usage(void);

using namespace std;
using namespace boxedin;


int main(int argc, char* argv[])
{
    char* stats_path = NULL;

#if defined (__linux__) || defined (__APPLE__)
    // Setup process signal handlers
    signal(SIGSEGV, signal_handler);
#endif

    // Handle command line options
    while (1)
    {
        const char* short_options = "s:c";

        int c = getopt(argc, argv, short_options);

        // No more options
        if (c == -1)
            break;

        switch (c)
        {
        case 's':
            stats_path = optarg;
            break;
        case 'c':
            boxedin::io::use_colors = true;
            break;
        default:
            usage();
        }
    }

    // Handle remaining command line argument(s).
    // If argument exists, its the level file; if no argument was given
    // expect the level to come from stdin.
    char* filename = NULL;

    if (optind < argc)
    {
        filename = argv[optind];
    }

    // Parse the level
    vector<vector<char> > charmap;
    if (filename)
    {
        ifstream level_istream(filename);
        boxedin::io::ParseCharMap(level_istream, charmap);
    }
    else
    {
        boxedin::io::ParseCharMap(cin, charmap);
    }

    if (!boxedin::io::IsValidBoxedInLevel(charmap))
    {
        fprintf(stderr, "ERROR: Invalid boxed in level\n");
        return 1;
    }

    if (filename)
    {
        cerr << filename << endl;
    }
    cerr << "Boxed In Level:" << endl;
    PrintCharMapInColor(cerr, charmap);

    // TODO: trim unnecessary rows and columns and re-print the level
    
    // A* Search
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    cerr << asctime(timeinfo) << endl;

    fprintf(stderr, "making level\n");
    Level level = Level::MakeLevel(charmap);

    ShortestManhattenDistanceThroughGearsToExitHeuristic heuristic(level);
    SearchResult result = astar(level, heuristic);
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    cerr << asctime(timeinfo) << endl;

    if (result.success)
    {
        cerr << "A* search succeeded" << endl;
        // write stats to file or stderr
        if (stats_path)
        {
            ofstream stats_output(stats_path);
            stats_output << result << endl;
        }
        else
        {
            cerr << result << endl;
        }

        // write solution to stdout
        cout << result.solution << endl;

        return 0;
    }

    cerr << "A* search failed" << endl;
    return 1;
}



static void usage(void)
{
    fprintf(stderr,
        "Usage:\n"
        "BoxedInSolver OPTIONS file\n"
        "  Reads a boxed in level from file and runs the solving\n"
        "  algorithm for it.\n"
        "  OPTIONS\n"
        "    -c\n"
        "      Print console output in color\n"
        "    -s, <statsfile>\n"
        "      Print solver stats to statsfile\n"
        );
    exit(1);
}


#if defined (__linux__) || defined (__APPLE__)
static void signal_handler(int signum)
{
    /* Print backtrace for seg fault */
    if (signum == SIGSEGV)
    {
        void* tracePtrs[100];
        int count = backtrace( tracePtrs, 100 );
        char** funcNames = backtrace_symbols( tracePtrs, count );

        // Print the stack trace
        for( int ii = 0; ii < count; ii++ )
            cerr << funcNames[ii] << endl;;

        // Free the string pointers
        free( funcNames );
        exit(1);
    }
}
#endif
