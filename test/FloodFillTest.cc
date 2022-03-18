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
  
  list<Action> expectedActions;
  expectedActions.push_back(Action::MakeAction("RR", {4, 6}));
  expectedActions.push_back(Action::MakeAction("URR", {4, 5}));
  expectedActions.push_back(Action::MakeAction("DRR", {4, 7}));
  expectedActions.sort();

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  actions.sort();
  delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 3);
  // FIXME: get this test working
  // EXPECT_EQ(actions, expectedActions);
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

  list<Action> expectedActions;
  expectedActions.push_back(Action::MakeAction("RR", {4, 6}));
  expectedActions.push_back(Action::MakeAction("URR", {4, 5}));
  expectedActions.push_back(Action::MakeAction("DRR", {4, 7}));
  expectedActions.sort();

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  actions.sort();
  delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 2);
  // FIXME: get this test working
  // EXPECT_EQ(actions, expectedActions);
}