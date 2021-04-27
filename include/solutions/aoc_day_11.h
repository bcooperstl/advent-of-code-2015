#ifndef __AOC_DAY_11__
#define __AOC_DAY_11__

#include "aoc_day.h"

#define MAX_PASSWORD_LEN 8

class AocDay11 : public AocDay
{
    private:
        string read_input(string filename);
        void generate_first_next_password(char * input, int input_len);
        void generate_next_password(char * input, int input_len);
        bool is_valid_password(char * input, int input_len);
    public:
        AocDay11();
        ~AocDay11();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
