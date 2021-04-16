#ifndef __AOC_DAY_5__
#define __AOC_DAY_5__

#include <regex>

#include "aoc_day.h"

class AocDay5 : public AocDay
{
    private:
        static regex rule_1_regex;
        static regex rule_2_regex;
        static regex rule_3_regex;
        static regex rule_4_regex;
        static regex rule_5_regex;

        vector<string> read_input(string filename);
        bool is_nice(string str);
        bool is_nice_complex(string str);
    public:
        AocDay5();
        ~AocDay5();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
