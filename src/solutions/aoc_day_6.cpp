#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_6.h"
#include "file_utils.h"

using namespace std;

Day6Field::Day6Field()
{
    for (int y=0; y<FIELD_SIZE_Y; y++)
    {
        for (int x=0; x<FIELD_SIZE_X; x++)
        {
            m_field[y][x]=false;
        }
    }
}

Day6Field::~Day6Field()
{
}

int Day6Field::count_elements(bool status)
{
    int count = 0;
    for (int y=0; y<FIELD_SIZE_Y; y++)
    {
        for (int x=0; x<FIELD_SIZE_X; x++)
        {
            if (m_field[y][x]==status)
            {
                count++;
            }
        }
    }
    return count;
}    

void Day6Field::process_instruction(Day6Instruction instruction)
{
    int counter=0;
    cout << "Instruction to ";
    switch (instruction.operation)
    {
        case TurnOn:
            cout << "turn on";
            break;
        case TurnOff:
            cout << "turn off";
            break;
        case Toggle:
            cout << "toggle";
            break;
    }
    cout << " cells from " << instruction.min_x << " <= x <= " << instruction.max_x << " to " << instruction.min_y << " <= y <= " << instruction.max_y << endl;
    
    for (int y=instruction.min_y; y<=instruction.max_y; y++)
    {
        for (int x=instruction.min_x; x<=instruction.max_x; x++)
        {
            if (instruction.operation==TurnOn)
            {
                m_field[y][x]=true;
            }
            else if (instruction.operation==TurnOff)
            {
                m_field[y][x]=false;
            }
            else // (instruction.operation==Toggle)
            {
                m_field[y][x]=!m_field[y][x];
            }
            counter++;
        }
    }
    cout << "  Resulted in " << counter << " cells touched" << endl;
}

AocDay6::AocDay6():AocDay(6)
{
}

AocDay6::~AocDay6()
{
}

/*
    "turn on 0,0 through 999,999" would turn on (or leave on) every light.
    "toggle 0,0 through 999,0" would toggle the first line of 1000 lights, turning off the ones that were on, and turning on the ones that were off.
    "turn off 499,499 through 500,500" would turn off (or leave off) the middle four lights.
*/
vector<Day6Instruction> AocDay6::parse_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    vector<Day6Instruction> data;
    char delims[2];
    delims[0]=' ';
    delims[1]=',';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return data;
    }
        
    for (int i=0; i<lines.size(); i++)
    {
        vector<string> words = lines[i];
        Day6Instruction instruction;
        if (words[0] == "turn" && words[1] == "on")
        {
            instruction.operation=TurnOn;
            instruction.min_x=strtol(words[2].c_str(), NULL, 10);
            instruction.min_y=strtol(words[3].c_str(), NULL, 10);
            instruction.max_x=strtol(words[5].c_str(), NULL, 10);
            instruction.max_y=strtol(words[6].c_str(), NULL, 10);
        }
        else if (words[0] == "turn" && words[1] == "off")
        {
            instruction.operation=TurnOff;
            instruction.min_x=strtol(words[2].c_str(), NULL, 10);
            instruction.min_y=strtol(words[3].c_str(), NULL, 10);
            instruction.max_x=strtol(words[5].c_str(), NULL, 10);
            instruction.max_y=strtol(words[6].c_str(), NULL, 10);
        }
        else if (words[0] == "toggle")
        {
            instruction.operation=Toggle;
            instruction.min_x=strtol(words[1].c_str(), NULL, 10);
            instruction.min_y=strtol(words[2].c_str(), NULL, 10);
            instruction.max_x=strtol(words[4].c_str(), NULL, 10);
            instruction.max_y=strtol(words[5].c_str(), NULL, 10);
        }
        else
        {
            cerr << "*****Invalid instruction on line " << i << endl;
            continue;
        }
        data.push_back(instruction);
    }
    
    return data;
}


string AocDay6::part1(string filename, vector<string> extra_args)
{
    Day6Field field;
    vector<Day6Instruction> instructions = parse_input(filename);
    
    for (int i=0; i<instructions.size(); i++)
    {
        field.process_instruction(instructions[i]);
    }
    
    ostringstream out;
    out << field.count_elements(true);
    return out.str();
}
