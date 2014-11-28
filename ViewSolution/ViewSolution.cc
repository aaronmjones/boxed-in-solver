/**
 * \file ViewSolution.cc
 * \brief This file contains the main() function for ViewSolution.
 */

#if defined (_WIN32)
#include <wingetopt.h>
#elif defined (__linux__)
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
    LevelState initialLevelState;
    boxedin::io::ParseLevel(level_istream, initialLevelState);

    /* Parse the solution */
    Path path;
    boxedin::io::ParseSolution(solution_istream, path);

    LevelState currentLevelState(initialLevelState);
    charmap lvlmap;
    while (run_count == -1 || run_index < run_count)
    {
        // Reset level state
        currentLevelState = initialLevelState;
        LevelState::MakeLevelMap(lvlmap, currentLevelState);

        int move_index = 0;
        int move_count = (int)path.size();

        cout << clear_screen;
        cout << "Move " << move_index++ << " / " << move_count << endl;
        cout << lvlmap << endl;
        this_thread::sleep_for(chrono::seconds(1));

        for (Path::iterator it = path.begin(); it != path.end(); ++it)
        {
            char c = toupper(*it);
            switch (c)
            {
            case 'U':
                currentLevelState.MoveUp();
                break;
            case 'D':
                currentLevelState.MoveDown();
                break;
            case 'L':
                currentLevelState.MoveLeft();
                break;
            case 'R':
                currentLevelState.MoveRight();
                break;
            default:
                cerr << "Unrecognized char in path file: " << c << endl;
                exit(1);
            }
            currentLevelState.PickupGear();

            LevelState::MakeLevelMap(lvlmap, currentLevelState);
            cout << clear_screen;
            cout << "Move " << move_index++ << " / " << move_count << endl;
            cout << lvlmap << endl;
            cout.flush();

            this_thread::sleep_for(chrono::milliseconds(250));
        }

        this_thread::sleep_for(chrono::seconds(2));
        if (run_index != -1)
        {
            run_index++;
        }
    }

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
