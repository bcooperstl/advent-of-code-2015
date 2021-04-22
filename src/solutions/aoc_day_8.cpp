#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_8.h"
#include "file_utils.h"

using namespace std;

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

vector<string> AocDay8::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return lines;
}

int AocDay8::calculate_literal_length(string input)
{
    return input.length();
}

int AocDay8::calculate_memory_length(string input)
{
    int length = 0;
    const char * str = input.c_str();
    for (int i=1; i<input.length()-1; i++) // skip first and last quote characters
    {
        if (str[i]=='\\')
        {
            if (str[i+1] == '\\' || str[i+1] == '\"')
            {
                length++;
                i++; // skip the one extra character - either the \ or the "
            }
            else if (str[i+1] == 'x')
            {
                length++;
                i+=3; // skip the x and the two following hex characters 
            }
            else
            {
                cerr << "****Invalid string [" << input << "]  FIX YOUR LOGIC" << endl;
            }
        }
        else
        {
            length++;
        }
    }
    
    return length;
}

int AocDay8::calculate_encoded_length(string input)
{
    int length = 2; // add 2 for the first and final quotes in the new string
    const char * str = input.c_str();
    for (int i=0; i<input.length(); i++)
    {
        if (str[i] == '\"')
        {
            length += 2; // one for the extra \ and one for the "
        }
        else if (str[i] == '\\')
        {
            length += 2; // one for the extra \ and one for the given backslash
        }
        else
        {
            length++;
        }
    }
    
    return length;
}

string AocDay8::part1(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    int sum_literal = 0;
    int sum_memory = 0;
    for (int i=0; i<input.size(); i++)
    {
        int literal = calculate_literal_length(input[i]);
        int memory  = calculate_memory_length(input[i]);
        cout << input[i] << " has literal length " << literal << " and memory length " << memory << endl;
        sum_literal += literal;
        sum_memory  += memory;
    }
    
    int result = sum_literal - sum_memory;
    cout << "Final result = sum_literal - sum_memory = " << sum_literal << " - " << sum_memory << " = " << result << endl;
    ostringstream out;
    out << result;
    return out.str();
}

string AocDay8::part2(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    int sum_literal = 0;
    int sum_encoded = 0;
    for (int i=0; i<input.size(); i++)
    {
        int literal = calculate_literal_length(input[i]);
        int encoded = calculate_encoded_length(input[i]);
        cout << input[i] << " has literal length " << literal << " and encoded length " << encoded << endl;
        sum_literal += literal;
        sum_encoded += encoded;
    }
    
    int result = sum_encoded - sum_literal;
    cout << "Final result = sum_encoded - sum_literal = " << sum_encoded << " - " << sum_literal << " = " << result << endl;
    ostringstream out;
    out << result;
    return out.str();
}
