#include "gtest/gtest.h"
#include "Heuristic.h"

using namespace boxedin;
using namespace testing;

TEST(SymmetricCostTableTest, GetCostTest) {
    SymmetricCostTable table(10); // 10x10 table
    //
    //     0 1 2 3 4 5 6 7 8 9
    //     -------------------
    // 0 - 0 1 2 3 4 5 6 7 8 9
    // 1 - 1 0 2 3 4 5 6 7 8 9
    // 2 - 2 2 0 3 4 5 6 7 8 9
    // 3 - 3 3 3 0 4 5 6 7 8 9
    // 4 - 4 4 4 4 0 5 6 7 8 9
    // 5 - 5 5 5 5 5 0 6 7 8 9
    // 6 - 6 6 6 6 6 6 0 7 8 9
    // 7 - 7 7 7 7 7 7 7 0 8 9
    // 8 - 8 8 8 8 8 8 8 8 0 9
    // 9 - 9 9 9 9 9 9 9 9 9 0

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (y > x)
            {
                continue;
            }
            if (x == y)
            {
                table.Cost(x, y, 0);
                EXPECT_EQ(0, *table.Cost(x, y));
            }
            else
            {
                table.Cost(x, y, x);
                EXPECT_EQ(x, *table.Cost(x, y));
            }
        }
    }

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            cost_t* cost = table.Cost(x, y);
            if (x == y)
            {
                EXPECT_EQ(*cost, (cost_t)0);
            }
            else if (x > y)
            {
                EXPECT_EQ(*cost, (cost_t)x);
            }
            else
            {
                EXPECT_EQ(*cost, (cost_t)y);
            }
        }
    }

#if 1
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            cost_t* cost = table.Cost(x, y);
            fprintf(stderr, " %d", *cost);
            if (x % 10 == 9)
            {
                fprintf(stderr, "\n");
            }
        }
    }
#endif    
}
