#ifndef __AOC_DAY_6__
#define __AOC_DAY_6__

#include "aoc_day.h"

#define FIELD_SIZE_X 1000
#define FIELD_SIZE_Y 1000


namespace Day6
{
    enum Operation
    {
        TurnOn,
        TurnOff,
        Toggle
    };
    
    
    struct Instruction
    {
        Operation operation;
        int min_x;
        int max_x;
        int min_y;
        int max_y;
    };
    
    class Field
    {
        private:
            bool m_field[FIELD_SIZE_Y][FIELD_SIZE_X]; // y index first, then x index
        public:
            Field();
            ~Field();
            int count_elements(bool status);
            void process_instruction(Instruction instruction);    
    };
    
    class BrightnessField
    {
        private:
            int m_field[FIELD_SIZE_Y][FIELD_SIZE_X]; // y index first, then x index
        public:
            BrightnessField();
            ~BrightnessField();
            long sum_brightness();
            void process_instruction(Instruction instruction);    
    };
}

using namespace Day6;

class AocDay6 : public AocDay
{
    private:
        vector<Instruction> parse_input(string filename);
    public:
        AocDay6();
        ~AocDay6();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
