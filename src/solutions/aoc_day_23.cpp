#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <map>

#include "aoc_day_23.h"
#include "file_utils.h"

using namespace std;
using namespace Day23;

AocDay23::AocDay23():AocDay(23)
{
}

AocDay23::~AocDay23()
{
}


/* Input format:

    'hlf r' sets register r to half its current value, then continues with the next instruction.
    'tpl r' sets register r to triple its current value, then continues with the next instruction.
    'inc r' increments register r, adding 1 to it, then continues with the next instruction.
    'jmp offset' is a jump; it continues with the instruction offset away relative to itself.
    'jie r, offset' is like jmp, but only jumps if register r is even ("jump if even").
    'jio r, offset' is like jmp, but only jumps if register r is 1 ("jump if one", not odd).

*/

void AocDay23::parse_input(string filename, Program & program)
{
    map<string, int> opcode_map;
    opcode_map["hlf"]=OPCODE_HLF;
    opcode_map["tpl"]=OPCODE_TPL;
    opcode_map["inc"]=OPCODE_INC;
    opcode_map["jmp"]=OPCODE_JMP;
    opcode_map["jie"]=OPCODE_JIE;
    opcode_map["jio"]=OPCODE_JIO;
    
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing commas.
    char delims[2];
    delims[0]=' ';
    delims[1]=',';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    for (int i=0; i<lines.size(); i++)
    {
        program.instructions[i].opcode = opcode_map[lines[i][0]];
        
        switch (program.instructions[i].opcode)
        {
            case OPCODE_HLF:
            case OPCODE_TPL:
            case OPCODE_INC:
                program.instructions[i].reg = lines[i][1][0] - 'a'; // the register is only one character long
                break;
            case OPCODE_JMP:
                program.instructions[i].offset = strtol(lines[i][1].c_str(), NULL, 10);
                break;
            case OPCODE_JIE:
            case OPCODE_JIO:
                program.instructions[i].reg = lines[i][1][0] - 'a'; // the register is only one character long
                program.instructions[i].offset = strtol(lines[i][2].c_str(), NULL, 10);
                break;
            default:
                cerr << "INVALID OPCODE. FIX YOUR PROGRAM" << endl;
        }
    }
    
    program.num_instructions = lines.size();
    
    return;
}

void AocDay23::init_program(Program & program)
{
    program.next_instruction = 0;
    program.reg[0] = 0;
    program.reg[1] = 0;
    
    return;
}

void AocDay23::run_program_to_end(Program & program)
{
    while ((program.next_instruction >= 0) && (program.next_instruction < program.num_instructions))
    {
        int ip = program.next_instruction;
        int reg = program.instructions[ip].reg;
        int offset = program.instructions[ip].offset;
        
        switch (program.instructions[ip].opcode)
        {
            // 'hlf r' sets register r to half its current value, then continues with the next instruction.
            case OPCODE_HLF:
                cout << "Line " << ip << " HLF halfing register " << reg << " from " << program.reg[reg] << " to " << program.reg[reg] / 2ull << endl;
                program.reg[reg] = program.reg[reg] / 2ull;
                program.next_instruction++;
                break;

            // 'tpl r' sets register r to triple its current value, then continues with the next instruction.
            case OPCODE_TPL:
                cout << "Line " << ip << " TPL tripling register " << reg << " from " << program.reg[reg] << " to " << program.reg[reg] * 3ull << endl;
                program.reg[reg] = program.reg[reg] * 3ull;
                program.next_instruction++;
                break;

            // 'inc r' increments register r, adding 1 to it, then continues with the next instruction.
            case OPCODE_INC:
                cout << "Line " << ip << " INC incrementing register " << reg << " from " << program.reg[reg] << " to " << program.reg[reg] + 1ull << endl;
                program.reg[reg] = program.reg[reg] + 1ull;
                program.next_instruction++;
                break;

            // 'jmp offset' is a jump; it continues with the instruction offset away relative to itself.
            case OPCODE_JMP:
                cout << "Line " << ip << " JMP jumping by " << offset << " to " << ip+offset <<  endl;
                program.next_instruction += offset;
                break;

            // 'jie r, offset' is like jmp, but only jumps if register r is even ("jump if even").
            case OPCODE_JIE:
                if (program.reg[reg] % 2 == 0)
                {
                    cout << "Line " << ip << " JIE is even - jumping by " << offset << " to " << ip+offset <<  endl;
                    program.next_instruction += offset;
                }
                else
                {
                    cout << "Line " << ip << " JIE is odd - no jump applied" << endl;
                    program.next_instruction++;
                }
                break;
            
            // 'jio r, offset' is like jmp, but only jumps if register r is 1 ("jump if one", not odd).
            case OPCODE_JIO:
                if (program.reg[reg] == 1)
                {
                    cout << "Line " << ip << " JIE is one - jumping by " << offset << " to " << ip+offset <<  endl;
                    program.next_instruction += offset;
                }
                else
                {
                    cout << "Line " << ip << " JIE is not one - no jump applied" << endl;
                    program.next_instruction++;
                }
                break;
            default:
                cerr << "INVALID OPCODE AT LINE " << ip << ". FIX YOUR PROGRAM" << endl;
        }
    }

    return;
}

string AocDay23::part1(string filename, vector<string> extra_args)
{
    Program program;
    
    parse_input(filename, program);
    
    init_program(program);
    
    run_program_to_end(program);
    
    ostringstream out;
    out << program.reg[1]; // get reg b
    return out.str();
}
