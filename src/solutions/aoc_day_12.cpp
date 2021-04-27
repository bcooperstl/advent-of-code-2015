#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_12.h"
#include "file_utils.h"

using namespace std;

AocDay12::AocDay12():AocDay(12)
{
}

AocDay12::~AocDay12()
{
}

string AocDay12::read_input(string filename)
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

// extract an optional negative sign and then one or more digits
regex AocDay12::numbers_regex("(\\-?\\d+)");

vector<int> AocDay12::get_all_numbers(string input)
{
    vector<int> numbers;
    match_results<std::string::const_iterator> matches;
    
    int counter = 0;
    for (sregex_iterator number_iter = sregex_iterator(input.begin(), input.end(), numbers_regex); number_iter != sregex_iterator(); number_iter++)
    {
        smatch number_match = *number_iter;
        int number = strtol(number_match.str(0).c_str(), NULL, 10);
        numbers.push_back(number);
        counter++;
        cout << "Matched [" << number_match.str(0) << "] with value " << number << " at position " << number_match.position(0) << endl;
    }
    
    return numbers;
}

string AocDay12::part1(string filename, vector<string> extra_args)
{
    string json = read_input(filename);
    vector<int> numbers = get_all_numbers(json);
    cout << "There were " << numbers.size() << " numbers found in the input json" << endl;
    
    int sum = 0;
    for (int i=0; i<numbers.size(); i++)
    {
        sum+=numbers[i];
    }
    
    ostringstream out;
    out << sum;
    return out.str();
}
