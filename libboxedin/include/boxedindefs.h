/**
 * \file boxedindefs.h
 * \brief This file contains macro definitions.
 */
#ifndef BOXED_IN_DEFS_H__
#define BOXED_IN_DEFS_H__


// Board map width and height
#define WIDTH 10
#define HEIGHT 14

// Define max number of gears allowed
#define GEARS_8 8
#define GEARS_16 16
#define GEARS_32 32
#define GEARS_MAX GEARS_16

// All valid level characters
#define SPACE         '\''
#define FLOOR         ' '
#define WALL          'x'
#define BOX           '+'
#define SWITCH_RED    'r'
#define SWITCH_GREEN  'g'
#define SWITCH_BLUE   'b'
#define SWITCH_YELLOW 'y'
#define GATE_RED      'R'
#define GATE_GREEN    'G'
#define GATE_BLUE     'B'
#define GATE_YELLOW   'Y'
#define GEAR          '*'
#define EXIT          '@'
#define PLAYER        'p'
#define FILLED        '-' // Reserved for Flood Fill algorithm

#define IS_FLOOR(c) \
    ( (c == FLOOR) || (c == FILLED) )

#define IS_SWITCH(c) \
    ( (c == SWITCH_RED)   || \
      (c == SWITCH_GREEN) || \
      (c == SWITCH_BLUE)  || \
      (c == SWITCH_YELLOW) )

#define IS_BOX_MOVE(c1,c2) \
    ( (c1 == BOX) && (IS_FLOOR(c2) || IS_SWITCH(c2) || (c2 == PLAYER)) )

#define CAN_FLOOD_FILL(c) \
    ( (c == PLAYER) || (c == FLOOR) || (c == GEAR) || (c == EXIT) || IS_SWITCH(c) )

// The offset between switch char and gate char.
// The offset between any uppercase and lowercase letter is constant.
#define KEY_OFFSET ('r' - 'R')
#define SWITCH_TO_GATE_CHAR(c) ( c - KEY_OFFSET )
#define GATE_TO_SWITCH_CHAR(c) ( c + KEY_OFFSET )

#define IS_SET(bit,flags) ((bit&flags)==(bit))



#define COLOR_TO_SWITCH_CHAR(c) \
        c == RED    ? 'r' :     \
        c == YELLOW ? 'y' :     \
        c == GREEN  ? 'g' :     \
        c == BLUE   ? 'b' : 'b'
#define COLOR_TO_GATE_CHAR(c)   \
        c == RED    ? 'R' :     \
        c == YELLOW ? 'Y' :     \
        c == GREEN  ? 'G' :     \
        c == BLUE   ? 'B' : 'B'
#define SWITCH_CHAR_TO_COLOR(c) \
        c == 'r'? RED    :      \
        c == 'y'? YELLOW :      \
        c == 'g'? GREEN  :      \
        c == 'b'? BLUE   : BLUE
#define GATE_CHAR_TO_COLOR(c)   \
        c == 'R'? RED    :      \
        c == 'Y'? YELLOW :      \
        c == 'G'? GREEN  :      \
        c == 'B'? BLUE   : BLUE


#endif
