#ifndef __AOC_DAY_3__
#define __AOC_DAY_3__

#include "aoc_day.h"

#define INPUT_NORTH '^'
#define INPUT_SOUTH 'v'
#define INPUT_WEST '<'
#define INPUT_EAST '>'

#define MAP_EMPTY ' '
#define MAP_VISITED '#'

class AocDay3 : public AocDay
{
    private:
        string read_input(string filename);
        void calculate_edges(string directions, int & min_x, int & max_x, int & min_y, int & max_y);
    public:
        AocDay3();
        ~AocDay3();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
