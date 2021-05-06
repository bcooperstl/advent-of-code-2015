#ifndef __AOC_DAY_16__
#define __AOC_DAY_16__

#include <map>

#include "aoc_day.h"

#define UNDEF_QUANTITY -9999
#define TARGET_SUE_NUMBER 0

namespace Day16
{
    class Sue
    {
        private:
            int m_number;
            int m_children;
            int m_cats;
            int m_samoyeds;
            int m_pomeranians;
            int m_akitas;
            int m_vizslas;
            int m_goldfish;
            int m_trees;
            int m_cars;
            int m_perfumes;

            void set_children(int value);
            void set_cats(int value);
            void set_samoyeds(int value);
            void set_pomeranians(int value);
            void set_akitas(int value);
            void set_vizslas(int value);
            void set_goldfish(int value);
            void set_trees(int value);
            void set_cars(int value);
            void set_perfumes(int value);
        public:
            Sue(int number);
            ~Sue();
            int get_number();
            void set_value(string field, int value);
            bool is_match(const Sue & other);
    };
}

using namespace Day16;

class AocDay16 : public AocDay
{
    private:
        void parse_input(string filename, vector<Sue> & possiblilities);
        Sue create_target_sue();
    public:
        AocDay16();
        ~AocDay16();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
