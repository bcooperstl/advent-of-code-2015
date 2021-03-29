#ifndef __AOC_DAY_2__
#define __AOC_DAY_2__

#include "aoc_day.h"

class AocDay2 : public AocDay
{
    private:
        vector<vector<long>> read_input(string filename);
        long calculate_wrapping_paper(long length, long width, long height);
    public:
        AocDay2();
        ~AocDay2();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
