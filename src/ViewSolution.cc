/**
 * \file ViewSolution.cc
 * \brief This file contains the main() function for ViewSolution.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */

#if defined (_WIN32)
#include <wingetopt.h>
#elif defined (__linux__) || defined (__APPLE__)
#include <getopt.h>
#endif

#include <cstdlib>
#include <cstdio>

#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

#include "boxedinio.h"
#include "boxedintypes.h"

using namespace std;
using namespace boxedin;

void usage(void);

int main(int argc, char* argv[])
{
#if 1
    /* run count -1 means infinite */
    int run_count = -1;
    int run_index = 0;

    cout << clear_screen;

    /* Handle command line options */
    while (1)
    {
        static const char* short_options = "n:c";

        int c = getopt( argc, argv, short_options );

        /* No more options */
        if (c == -1)
            break;

        switch (c)
        {
        case 'n':
            run_count = atoi(optarg);
            break;
        case 'c':
            boxedin::io::use_colors = true;
            break;
        default:
            usage();
        }
    }

    /* Handle remaining command line arguments */
    char* level_path = NULL;
    char* solution_path = NULL;

    // FIXME: error if level path and solution path not given
    
    if (optind < argc)
    {
        level_path = argv[optind++];
    }

    if (optind < argc)
    {
        solution_path = argv[optind];
    }

    ifstream level_istream(level_path);
    ifstream solution_istream(solution_path);

    /* Parse the level */
    vector<vector<char> > charmap;
    boxedin::io::ParseCharMap(level_istream, charmap);
    Level level = Level::MakeLevel(charmap);

    /* Parse the solution */
    Path path;
    boxedin::io::ParseSolution(solution_istream, path);

    while (run_count == -1 || run_index < run_count)
    {
      // Reset level state
      Level currentLevel = level;
      charmap = currentLevel.Map();

      int move_index = 0;
      int move_count = (int)path.size();

      cout << clear_screen;
      cout << "Move " << move_index++ << " / " << move_count << endl;
//      cout << charmap << endl;
      PrintCharMapInColor(cout, charmap);
      this_thread::sleep_for(chrono::seconds(1));

      for (Path::iterator it = path.begin(); it != path.end(); ++it)
      {
        char c = toupper(*it);
        switch (c)
        {
        case 'U':
          currentLevel.MoveUp();
          break;
        case 'D':
          currentLevel.MoveDown();
          break;
        case 'L':
          currentLevel.MoveLeft();
          break;
        case 'R':
          currentLevel.MoveRight();
          break;
        default:
          cerr << "Unrecognized char in path file: " << c << endl;
          exit(1);
        }
        currentLevel.TryPickupGear();

        charmap = currentLevel.Map();
        cout << clear_screen;
        cout << "Move " << move_index++ << " / " << move_count << endl;
//        cout << charmap << endl;
        PrintCharMapInColor(cout, charmap);
        cout.flush();

        this_thread::sleep_for(chrono::milliseconds(250));
      }

      this_thread::sleep_for(chrono::seconds(2));
      if (run_index != -1)
      {
        run_index++;
      }
    }
#endif
    return 0;
}

void usage(void)
{
    fprintf(stderr,
            "Usage:\n"
            "ViewSolution OPTIONS level solution\n"
            "\n"
            "OPTIONS\n"
            "  n NUM\n"
            "    View solution NUM times.\n"
            "    -1 will loop forever (default).\n"
            "  c\n"
            "    Print console output using color.\n"
            "\n"
        );
    exit(1);
}
