#ifndef __AOC_DAY_18__
#define __AOC_DAY_18__

#include "aoc_day.h"
#include "screen.h"

#define LIGHT_ON '#'
#define LIGHT_OFF '.'

class AocDay18 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        void run_rules(Screen * screen);
    public:
        AocDay18();
        ~AocDay18();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
