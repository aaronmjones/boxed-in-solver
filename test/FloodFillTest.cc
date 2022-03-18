#include <gtest/gtest.h>
#include <iostream>
#include <astar.h>
#include <boxedinio.h>
#include <Node.h>

using namespace boxedin;
using namespace testing;

TEST(FloodFill, canFindExpectedActionsLevel1State1) {
  auto level = Level::MakeLevel(
      "''''''''''\n"
      "''''''''''\n"
      "''''''''''\n"
      "''''''''''\n"
      "'xxxxxxxx'\n"
      "'x  +  xx'\n"
      "'xp +  @x'\n"
      "'x  +  xx'\n"
      "'xxxxxxxx'\n"
      "''''''''''\n"
      "''''''''''\n"
      "''''''''''\n"
      "''''''''''\n"
      "''''''''''\n"
  );
  
  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  //delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 3);
}

TEST(FloodFill, FloodFill_canFindExpectedActionsLevel3State1)
{
  auto level = Level::MakeLevel(
      "''''''''''\n"
      "'xxxxx''''\n"
      "'x * x''''\n"
      "'x   x''''\n"
      "'x+++x''''\n"
      "'x x xxx''\n"
      "'x x Y@x''\n"
      "'x   xxx''\n"
      "'x x xx'''\n"
      "'x   yx'''\n"
      "'xxxx x'''\n"
      "''''xpx'''\n"
      "''''xxx'''\n"
      "''''''''''\n"
  );

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  //delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 2);
}