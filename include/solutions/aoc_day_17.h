#ifndef __AOC_DAY_17__
#define __AOC_DAY_17__

#include "aoc_day.h"

class AocDay17 : public AocDay
{
    private:
        void read_input(string filename, vector<long> & values);
        int number_of_combinations_to_target(vector<long> & values, int position, int target);
        void find_min_used_for_target_find_min(vector<long> & values, int position, int target, int num_used, int & min);
        int number_of_combinations_to_target_matching_count(vector<long> & values, int position, int target_value, int num_used, int target_count);
    public:
        AocDay17();
        ~AocDay17();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
