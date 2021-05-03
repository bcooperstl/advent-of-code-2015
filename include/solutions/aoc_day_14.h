#ifndef __AOC_DAY_14__
#define __AOC_DAY_14__

#include <map>

#include "aoc_day.h"

#define MAX_PEOPLE 10

namespace Day14
{
    struct Reindeer
    {
        string name;
        int flight_speed;
        int flight_duration;
        int rest_duration;
    };
}

using namespace Day14;

class AocDay14 : public AocDay
{
    private:
        void parse_input(string filename, vector<Reindeer> & reindeer);
        int calculate_distance_for_time(Reindeer reindeer, int time);
    public:
        AocDay14();
        ~AocDay14();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
