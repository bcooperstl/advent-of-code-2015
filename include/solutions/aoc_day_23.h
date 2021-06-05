#ifndef __AOC_DAY_23__
#define __AOC_DAY_23__

#include <string>

#include "aoc_day.h"

#define OPCODE_HLF 1
#define OPCODE_TPL 2
#define OPCODE_INC 3
#define OPCODE_JMP 4
#define OPCODE_JIE 5
#define OPCODE_JIO 6

#define MAX_PROGRAM_LENGTH 50

#define NUM_REG 2

namespace Day23
{
    struct Instruction
    {
        int opcode;
        int reg; // 0 for reg a, 1 for reg b
        int offset;
    };
    
    struct Program
    {
        Instruction instructions[MAX_PROGRAM_LENGTH];
        int num_instructions;
        int next_instruction;
        unsigned long long int reg[NUM_REG]; // 0 for reg a, 1 for reg b
    };
}

using namespace Day23;

class AocDay23 : public AocDay
{
    private:
        void parse_input(string filename, Program & program);
        
        void init_program(Program & program);
        
        void run_program_to_end(Program & program);
        
    public:
        AocDay23();
        ~AocDay23();
        
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif

