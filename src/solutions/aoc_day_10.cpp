#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_10.h"
#include "file_utils.h"

using namespace std;

AocDay10::AocDay10():AocDay(10)
{
}

AocDay10::~AocDay10()
{
}

string AocDay10::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return "";
    }
    
    return lines[0];
}

string AocDay10::run_step(string input)
{
    int counter = 1;
    char current = input[0];
    
    ostringstream out;
    
    for (int i=1; i<input.size(); i++)
    {
        if (input[i] == current)
        {
            counter++;
        }
        else
        {
            out << counter << current;
            counter = 1;
            current = input[i];
        }
    }
    out << counter << current;
    
    return out.str();
}

string AocDay10::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 10 Part 1 requires 1 extra argument for the number of steps" << endl;
        return "";
    }

    string input = read_input(filename);
    string output;
    int rounds = strtoul(extra_args[0].c_str(), NULL, 10);
    for (int i=1; i<=rounds; i++)
    {
        output = run_step(input);
        cout << "Round " << i << ": transforms input " << input << " to output " << output << endl;
        input = output;
    }
    
    ostringstream out;
    out << output.length();
    return out.str();
}
