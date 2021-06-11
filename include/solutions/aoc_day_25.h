#ifndef __AOC_DAY_25__
#define __AOC_DAY_25__

#include <string>

#include "aoc_day.h"

class AocDay25 : public AocDay
{
    private:
        void parse_input(string filename, int & row, int & column);
        
        unsigned long long find_value(int target_row, int target_column);
        
    public:
        AocDay25();
        ~AocDay25();
        
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif

