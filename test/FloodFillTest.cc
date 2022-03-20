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
  actions.sort();
  expectedActions.sort();
  list<Action> difference;
  set_difference(std::begin(expectedActions),
                 std::end(expectedActions),
                 std::begin(actions),
                 std::end(actions),
                 std::back_inserter(difference));
  // FIXME: Does not print lists as expected
  EXPECT_EQ(difference.size(), 0)
    << "EXPECTED:\n" << ::testing::PrintToString(expectedActions) << std::endl
    << "ACTUAL:\n" << ::testing::PrintToString(actions) << std::endl
    << "DIFFERENCE:\n" << ::testing::PrintToString(difference) << std::endl;
}

TEST(FloodFill, canFindExpectedActionsLevel3State1)
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
  actions.sort();
  expectedActions.sort();
  list<Action> difference;
  set_difference(std::begin(expectedActions),
                 std::end(expectedActions),
                 std::begin(actions),
                 std::end(actions),
                 std::back_inserter(difference));
  // FIXME: Does not print lists as expected
  EXPECT_EQ(difference.size(), 0)
    << "EXPECTED:\n" << ::testing::PrintToString(expectedActions) << std::endl
    << "ACTUAL:\n" << ::testing::PrintToString(actions) << std::endl
    << "DIFFERENCE:\n" << ::testing::PrintToString(difference) << std::endl;
}

TEST(FloodFill, canFindExpectedActionsLevel4State1)
{
  auto level = Level::MakeLevel(
      "''''''''''\n"
      "''xxx'''''\n"
      "''x@x'''''\n"
      "''xRxxxx''\n"
      "''x   *x''\n"
      "''xx r x''\n"
      "''xx  xx''\n"
      "''x  + x''\n"
      "''xx+++x''\n"
      "''x*   x''\n"
      "''x  p x''\n"
      "''xxxxxx''\n"
      "''''''''''\n"
      "''''''''''\n"
  );

  list<Action> expectedActions;
  expectedActions.push_back(Action::MakeAction("ULL", {3, 9}));
  expectedActions.push_back(Action::MakeAction("ULU", {4, 8}));
  expectedActions.push_back(Action::MakeAction("URU", {6, 8}));

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
  actions.sort();
  expectedActions.sort();
  list<Action> difference;
  set_difference(std::begin(expectedActions),
                 std::end(expectedActions),
                 std::begin(actions),
                 std::end(actions),
                 std::back_inserter(difference));
  // FIXME: Does not print lists as expected
  auto str = ::testing::PrintToString(expectedActions);
  EXPECT_EQ(difference.size(), 0)
    << "EXPECTED:\n" << ::testing::PrintToString(expectedActions) << std::endl
    << "ACTUAL:\n" << ::testing::PrintToString(actions)
    << "DIFFERENCE:\n" << ::testing::PrintToString(difference);
}