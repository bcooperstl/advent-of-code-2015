#ifndef __AOC_DAY_15__
#define __AOC_DAY_15__

#include <map>

#include "aoc_day.h"

#define MAX_INGREDIENTS 4

namespace Day15
{
    struct Ingredient
    {
        string name;
        long capacity;
        long durability;
        long flavor;
        long texture;
        long calories;
    };
}

using namespace Day15;

class AocDay15 : public AocDay
{
    private:
        int parse_input(string filename, Ingredient ** ingredients);
    public:
        AocDay15();
        ~AocDay15();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
