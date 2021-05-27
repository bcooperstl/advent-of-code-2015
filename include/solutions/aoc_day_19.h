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
        bool path_found_recursive(string input, vector<pair<string, string>> & reverse_rules, vector<string> & start_strings, int max_start_string_length, int num_replacements, int & best_replacements, set<string> & tried_and_false);
        bool path_found_2(string input, vector<pair<string, string>> & reverse_rules, int & best_replacements);
    public:
        AocDay19();
        ~AocDay19();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
        static bool sort_pair_length_order(const pair<string,string> &left, const pair<string,string> &right);
};


#endif
