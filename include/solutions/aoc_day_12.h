#ifndef __AOC_DAY_12__
#define __AOC_DAY_12__

#include <regex>

#include "aoc_day.h"

class AocDay12 : public AocDay
{
    private:
        static regex numbers_regex;

        string read_input(string filename);
        
        vector<int> get_all_numbers(string input);
        
        void remove_red(char * input, int input_length);

    public:
        AocDay12();
        ~AocDay12();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
