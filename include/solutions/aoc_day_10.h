#ifndef __AOC_DAY_10__
#define __AOC_DAY_10__

#include "aoc_day.h"

class AocDay10 : public AocDay
{
    private:
        string read_input(string filename);
        string run_step(string input);
    public:
        AocDay10();
        ~AocDay10();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
