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

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 3);

  // If action lists are equal, then the difference list will be empty.
  list<Action> difference;
  set_difference(std::begin(expectedActions),
                 std::end(expectedActions),
                 std::begin(actions),
                 std::end(actions),
                 std::back_inserter(difference));
  // FIXME: Does not print lists as expected
  EXPECT_EQ(difference.size(), 0)
    << "EXPECTED:\n" << ::testing::PrintToString(expectedActions)
    << "ACTUAL:\n" << ::testing::PrintToString(actions)
    << "DIFFERENCE:\n" << ::testing::PrintToString(difference);
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
  expectedActions.push_back(Action::MakeAction("UULUUUUU", {4, 4}));
  expectedActions.push_back(Action::MakeAction("UULUULLUUU", {2, 4}));

  ShortestDistanceThroughGearsToExitHeuristic heuristic(level);
  auto node = Node::MakeStartNode(level, heuristic);
  auto actions = find_actions(level, *node);
  delete node;
  std::cerr << level << std::endl;
  for (const auto& action : actions)
  {
      std::cerr << action << std::endl;
  }
  EXPECT_EQ(actions.size(), 2);

  // If action lists are equal, then the difference list will be empty.
  list<Action> difference;
  set_difference(std::begin(expectedActions),
                 std::end(expectedActions),
                 std::begin(actions),
                 std::end(actions),
                 std::back_inserter(difference));
  // FIXME: Does not print lists as expected
  EXPECT_EQ(difference.size(), 0)
    << "EXPECTED:\n" << ::testing::PrintToString(expectedActions)
    << "ACTUAL:\n" << ::testing::PrintToString(actions)
    << "DIFFERENCE:\n" << ::testing::PrintToString(difference);
}