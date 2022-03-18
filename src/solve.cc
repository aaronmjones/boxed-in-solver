/**
 * \file BoxedInSolver.cc
 * \brief This file contains the main() function for BoxedInSolver.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */

#if defined (__linux__) || defined (__APPLE__)
# include <signal.h>
# include <execinfo.h> // backtrace
#endif

#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "boxedinio.h"
#include "astar.h"
#include "Heuristic.h"
#include "Level.h"
#include "Node.h"


#if defined (__linux__) || defined (__APPLE__)
static void signal_handler(int signum);
#endif


using namespace std;
using namespace boxedin;


int main(int argc, char* argv[])
{
  string stats_path;
  string level_path;
  bool use_color = true;
  
#if defined (__linux__) || defined (__APPLE__)
  // Setup process signal handlers
  signal(SIGSEGV, signal_handler);
#endif

  try {
    boost::program_options::options_description desc(
      "solve OPTIONS <level-file>\nOPTIONS"
      );
    desc.add_options()
      ("help,h",                                                                  "Display help"                  )
      ("no-color,n",                                                              "Do not display level in color" )
      ("stats,s", boost::program_options::value<string>(&stats_path),             "Output stats file"             )
      ("level,l", boost::program_options::value<string>(&level_path)->required(), "Input boxed-in level file"     )
      ;
    
    boost::program_options::positional_options_description positionalOptions;
    positionalOptions.add("level", 1);
        
    boost::program_options::variables_map variablesMap;
    boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv)
      .options(desc)
      .positional(positionalOptions)
      .run(),
      variablesMap );

    if (variablesMap.count("help"))
    {
      cerr << desc << endl;
      return 0;
    }

    boost::program_options::notify(variablesMap);

    if (variablesMap.count("no-color"))
    {
      use_color = false;
    }
  }
  catch (boost::program_options::error& e)
  {
    cerr << e.what() << std::endl;
    return 1;
  }

  // ---------------------------------------------------------------------------

  // Parse the level
  vector<vector<char> > charmap;
  if (!level_path.empty())
  {
    cerr << level_path << endl;
    ifstream level_istream(level_path.c_str());
    boxedin::io::ParseCharMap(level_istream, charmap);
  }
  else
  {
    fprintf(stderr, "ERROR: Please specify a level file\n");
    return 1;
  }

  if (!boxedin::io::IsValidBoxedInLevel(charmap))
  {
    fprintf(stderr, "ERROR: Invalid boxed in level\n");
    return 1;
  }

  cerr << "Boxed In Level:" << endl;
  PrintCharMap(cerr, charmap, use_color);

  // TODO: trim unnecessary rows and columns and re-print the level
    
  // A* Search
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  cerr << asctime(timeinfo) << endl;

  Level level = Level::MakeLevel(charmap);

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  SearchResult result = astar(level, heuristic);
    
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  cerr << asctime(timeinfo) << endl;

  if (result.success)
  {
    cerr << "A* search succeeded" << endl;
    // write stats to file or stderr
    if (!stats_path.empty())
    {
      ofstream stats_output(stats_path.c_str());
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
