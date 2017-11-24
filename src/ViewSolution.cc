/**
 * \file ViewSolution.cc
 * \brief This file contains the main() function for ViewSolution.
 * \author Aaron Jones
 * \date 2014
 * \copyright GNU Public License.
 */

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <boost/program_options.hpp>
#include <boxedinio.h>
#include <boxedintypes.h>


using namespace std;
using namespace boxedin;


int main(int argc, char* argv[])
{
  string level_path;
  string solution_path;
  bool use_color = true;
  /* run count -1 means infinite */
  int run_count = -1;
  int run_index = 0;

  try {
    boost::program_options::options_description desc("view-solution OPTIONS <level-file> <solution-file>\nOPTIONS");
    desc.add_options()
      ("help,h",                                                                        "Display help"                   )
      ("no-color,n",                                                                    "Do not display output in color" )
      ("level,l",    boost::program_options::value<string>(&level_path)->required(),    "Input boxed-in level file"      )
      ("solution,s", boost::program_options::value<string>(&solution_path)->required(), "Input solution file"            )
      ("count,c",    boost::program_options::value<int>(&run_count),                    "View solution N times"          )
      ;
    
    boost::program_options::positional_options_description positionalOptions;
    positionalOptions.add("level", 1);
    positionalOptions.add("solution", 2);
        
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

  ifstream level_istream(level_path.c_str());
  ifstream solution_istream(solution_path.c_str());

  /* Parse the level */
  vector<vector<char> > charmap;
  boxedin::io::ParseCharMap(level_istream, charmap);
  Level level = Level::MakeLevel(charmap);

  /* Parse the solution */
  Path path;
  boxedin::io::ParseSolution(solution_istream, path);

  cout << clear_screen;

  while (run_count == -1 || run_index < run_count)
  {
    // Reset level state
    Level currentLevel = level;
    charmap = currentLevel.Map();

    int move_index = 0;
    int move_count = (int)path.size();

    cout << clear_screen;
    cout << "Move " << move_index++ << " / " << move_count << endl;
    PrintCharMap(cout, charmap, use_color);
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
      PrintCharMap(cout, charmap, use_color);
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

