#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_1.h"
#include "file_utils.h"

using namespace std;

AocDay1::AocDay1():AocDay(1)
{
}

AocDay1::~AocDay1()
{
}

string AocDay1::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return "";
    }
    return raw_lines[0];
}

string AocDay1::part1(string filename, vector<string> extra_args)
{
    string data = read_input(filename);
    long floor = 0;
    for (int i=0; i<data.size(); i++)
    {
        if (data[i] == '(')
        {
            floor++;
        }
        else if (data[i] == ')')
        {
            floor--;
        }
        else
        {
            cerr << "Invalid character " << data[i] << " read at position " << i << endl;
        }
    }
    
    ostringstream out;
    out << floor;
    return out.str();
}
