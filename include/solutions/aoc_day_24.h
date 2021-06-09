#ifndef __AOC_DAY_24__
#define __AOC_DAY_24__

#include <string>

#include "aoc_day.h"

#define MAX_VALUES 30

class AocDay24 : public AocDay
{
    private:
        vector<long> parse_input(string filename);
        long calculate_qr(long * weights, int num_weights);
        bool can_split_remainder_to_equal(long * values, int num_values, long target);
        bool next_position(long * input, int input_size, int output_size, long * values, int * positions, long & current_sum);
        bool find_best_partition(long * input, int input_size, int output_size, long target, long * best_values, long & best_qr);
    public:
        AocDay24();
        ~AocDay24();
        
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif

