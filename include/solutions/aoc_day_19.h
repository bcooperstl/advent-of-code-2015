#ifndef __AOC_DAY_19__
#define __AOC_DAY_19__

#include "aoc_day.h"

#include <set>
#include <map>

class AocDay19 : public AocDay
{
    private:
        void parse_input(string filename, map<string, vector<string>> & rules, string & input);
        set<string> run_1_round(string input, map<string, vector<string>> & rules);
    public:
        AocDay19();
        ~AocDay19();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
