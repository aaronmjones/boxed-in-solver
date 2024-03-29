/**
 * \file ViewSolution.cc
 * \brief This file contains the main() function for ViewSolution.
 * \author Aaron Jones
 * \date 2014-2022
 * \copyright GNU Public License.
 */

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include <string>
#include <thread>
#include <boost/program_options.hpp>
#include "boxedinio.h"
#include "boxedintypes.h"


using namespace std;
using namespace boxedin;
namespace po = boost::program_options;

void Print(vector<vector<char> > charmap, int move_count, vector<char> path, bool use_color)
{
  cout << "Move " << move_count << " / " << path.size() << endl;

  PrintCharMap(cout, charmap, use_color);

  // Print solution moves thus far
  for (size_t i=0; i<move_count; ++i)
    cout << path[i];
  cout << endl;
}

int main(int argc, char* argv[])
{
  string level_path;
  string solution_path;
  bool help = false;
  bool no_color = false;
  bool animate = false;
  bool animate_once = false;

  try {
    po::options_description desc("view-solution OPTIONS <level-file> <solution-file>\nOPTIONS");
    desc.add_options()
      ("help,h",                                                       "Display help"                   )
      ("no-color,n",     po::bool_switch(&no_color),                    "Do not display output in color" )
      ("animate,a",      po::bool_switch(&animate),                     "Print animated solution"        )
      ("animate-once,o", po::bool_switch(&animate_once),                "Print animated solution once"   )
      ("level,l",        po::value<string>(&level_path)->required(),    "Input boxed-in level file"      )
      ("solution,s",     po::value<string>(&solution_path)->required(), "Input solution file"            )
      ;
    
    po::positional_options_description positionalOptions;
    positionalOptions.add("level", 1);
    positionalOptions.add("solution", 2);
        
    po::variables_map variablesMap;
    po::store(
      po::command_line_parser(argc, argv)
      .options(desc)
      .positional(positionalOptions)
      .run(),
      variablesMap );

    if (help)
    {
      cerr << desc << endl;
      return 0;
    }

    po::notify(variablesMap);
  }
  catch (po::error& e)
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
  vector<char> path;
  boxedin::io::ParseSolution(solution_istream, path);
  bool use_color = !no_color;
  animate = animate || animate_once;

  do
  {
    // Reset level state
    Level currentLevel = level;
    charmap = currentLevel.Render();

    int move_count = 0;

    if (animate)
    {
      cout << clear_screen;
      Print(charmap, move_count, path, use_color);
      this_thread::sleep_for(chrono::seconds(1));
    }

    for (auto c : path)
    {
      move_count++;
      auto & coord = currentLevel.player_coord_;
      c = toupper(c);
      switch (c)
      {
      case 'U':
        if (Level::CanMoveUp(charmap, coord.x, coord.y))
          currentLevel.MoveUp();
        else
          throw std::runtime_error(fmt::format("Cannot move up from ({},{})", coord.x, coord.y));
        break;
      case 'D':
        if (Level::CanMoveDown(charmap, coord.x, coord.y))
          currentLevel.MoveDown();
        else
          throw std::runtime_error(fmt::format("Cannot move down from ({},{})", coord.x, coord.y));
        break;
      case 'L':
        if (Level::CanMoveLeft(charmap, coord.x, coord.y))
          currentLevel.MoveLeft();
        else
          throw std::runtime_error(fmt::format("Cannot move left from ({},{})", coord.x, coord.y));
        break;
      case 'R':
        if (Level::CanMoveRight(charmap, coord.x, coord.y))
          currentLevel.MoveRight();
        else
          throw std::runtime_error(fmt::format("Cannot move right from ({},{})", coord.x, coord.y));
        break;
      default:
        cerr << "Unrecognized char in path file: " << c << endl;
        exit(1);
      }
      currentLevel.TryPickupGear();

      charmap = currentLevel.Render();

      if (animate)
      {
        cout << clear_screen;
        Print(charmap, move_count, path, use_color);
        this_thread::sleep_for(chrono::milliseconds(500));
      }
    }

    if (animate)
    {
      this_thread::sleep_for(chrono::seconds(2));
    }
    else
    {
      // If we're not animating, then return success if level was solved.
      auto gearsLeft = Level::GearsLeft(charmap);
      if (gearsLeft != 0)
      {
        Print(charmap, move_count, path, use_color);
        fmt::print("Invalid solution: there are still {} gears remaining\n", gearsLeft);
        exit(1);
      }
      if (!(currentLevel.player_coord_ == currentLevel.exit_coord_))
      {
        Print(charmap, move_count, path, use_color);
        fmt::print("Invalid solution: player did not reach exit\n");
        exit(1);
      }
      Print(charmap, move_count, path, use_color);
      fmt::print("Solution is valid!\n");
      exit(0); // Success
    }

    animate = animate_once ? false : animate;
  } while (animate);

  return 0;
}

