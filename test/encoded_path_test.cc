#include "gtest/gtest.h"
#include "EncodedPath.h"

using namespace boxedin;
using namespace testing;

    // ENCODED_PATH_DIRECTION_UP = 0,
    // ENCODED_PATH_DIRECTION_RIGHT,
    // ENCODED_PATH_DIRECTION_DOWN,
    // ENCODED_PATH_DIRECTION_LEFT,

TEST(EncodedPath, accessorsReturnExpectedValues) {
    EncodedPath encoded_path;

    encoded_path.push_back( ENCODED_PATH_DIRECTION_UP );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_UP );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_DOWN );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_DOWN );

    encoded_path.push_back( ENCODED_PATH_DIRECTION_LEFT );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_RIGHT );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_LEFT );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_RIGHT );

    encoded_path.push_back( ENCODED_PATH_DIRECTION_UP );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_DOWN );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_RIGHT );
    encoded_path.push_back( ENCODED_PATH_DIRECTION_LEFT );

    
    EXPECT_EQ( encoded_path.at(0), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(1), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(2), ENCODED_PATH_DIRECTION_DOWN );
    EXPECT_EQ( encoded_path.at(3), ENCODED_PATH_DIRECTION_DOWN );

    EXPECT_EQ( encoded_path.at(4), ENCODED_PATH_DIRECTION_LEFT );
    EXPECT_EQ( encoded_path.at(5), ENCODED_PATH_DIRECTION_RIGHT );
    EXPECT_EQ( encoded_path.at(6), ENCODED_PATH_DIRECTION_LEFT );
    EXPECT_EQ( encoded_path.at(7), ENCODED_PATH_DIRECTION_RIGHT );

    EXPECT_EQ( encoded_path.at(8), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(9), ENCODED_PATH_DIRECTION_DOWN );
    EXPECT_EQ( encoded_path.at(10), ENCODED_PATH_DIRECTION_RIGHT );
    EXPECT_EQ( encoded_path.at(11), ENCODED_PATH_DIRECTION_LEFT );
}

TEST(EncodedPath, AccessorsReturnExpectedValuesWithStringConstructor)
{
    EncodedPath encoded_path("UUDDLRLRUDRL");

    EXPECT_EQ( encoded_path.at(0), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(1), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(2), ENCODED_PATH_DIRECTION_DOWN );
    EXPECT_EQ( encoded_path.at(3), ENCODED_PATH_DIRECTION_DOWN );

    EXPECT_EQ( encoded_path.at(4), ENCODED_PATH_DIRECTION_LEFT );
    EXPECT_EQ( encoded_path.at(5), ENCODED_PATH_DIRECTION_RIGHT );
    EXPECT_EQ( encoded_path.at(6), ENCODED_PATH_DIRECTION_LEFT );
    EXPECT_EQ( encoded_path.at(7), ENCODED_PATH_DIRECTION_RIGHT );

    EXPECT_EQ( encoded_path.at(8), ENCODED_PATH_DIRECTION_UP );
    EXPECT_EQ( encoded_path.at(9), ENCODED_PATH_DIRECTION_DOWN );
    EXPECT_EQ( encoded_path.at(10), ENCODED_PATH_DIRECTION_RIGHT );
    EXPECT_EQ( encoded_path.at(11), ENCODED_PATH_DIRECTION_LEFT );
}