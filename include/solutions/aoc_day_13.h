#ifndef __AOC_DAY_13__
#define __AOC_DAY_13__

#include <map>

#include "aoc_day.h"

#define MAX_PEOPLE 10

namespace Day13
{
    struct Person
    {
        string name;
        map<Person *, int> units;
    };
}

using namespace Day13;

class AocDay13 : public AocDay
{
    private:
        int parse_input(string filename, Person ** people); // returns the number of cities created
        int calculate_happiness(Person ** people, int num_people);
        void modified_heaps_algorithm_generate(Person ** people, int num_people, int k, int & best_case);
        int find_best_arrangement(Person ** people, int num_people);
        void dump_cycle(Person ** people, int num_people);
    public:
        AocDay13();
        ~AocDay13();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
        static int compare_people_by_name_func(const void *left, const void *right);
        static int compare_people_by_name(Person *left, Person *right);
};


#endif
