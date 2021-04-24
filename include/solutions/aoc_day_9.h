#ifndef __AOC_DAY_9__
#define __AOC_DAY_9__

#include <map>

#include "aoc_day.h"

#define MAX_CITIES 10

namespace Day9
{
    struct City
    {
        string name;
        map<City *, int> distances;
    };
}

using namespace Day9;

class AocDay9 : public AocDay
{
    private:
        int parse_input(string filename, City ** cities); // returns the number of cities created
        int calculate_distance(City ** cities, int num_cities);
        void heaps_algorithm_generate(City ** cities, int num_cities, int k, int & best_distance, int & worst_distnace);
        int find_shortest_journey(City ** cities, int num_cities);
        int find_longest_journey(City ** cities, int num_cities);
    public:
        AocDay9();
        ~AocDay9();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
