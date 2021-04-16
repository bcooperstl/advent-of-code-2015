#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_5.h"
#include "file_utils.h"

using namespace std;

AocDay5::AocDay5():AocDay(5)
{
}

AocDay5::~AocDay5()
{
}

vector<string> AocDay5::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return lines;
}

// check for 3 or more vowels. Going to search for [aeiou] and then get the count
regex AocDay5::rule_1_regex("([aeiou]{1})");

// contains a letter [a-z] that appears twice in a row.
regex AocDay5::rule_2_regex("([a-z])\\1{1}");

// does not contain 'ab' 'cd' 'pq' or 'xy'
regex AocDay5::rule_3_regex("ab|cd|pq|xy");

bool AocDay5::is_nice(string input)
{
    bool is_nice = true;
    cout << "Checking string [" << input << "]" << endl;
    match_results<std::string::const_iterator> matches;
    
    int counter = 0;
    for (sregex_iterator rule_1_iter = sregex_iterator(input.begin(), input.end(), rule_1_regex); rule_1_iter != sregex_iterator(); rule_1_iter++)
    {
        smatch match = *rule_1_iter;
        counter++;
        cout << "Match [" << match.str(0) << "] at position " << match.position(0) << endl;
    }
    
    if (counter >= 3)
    {
        cout << "  " << counter << " vowels makes the string nice" << endl;
    }
    else
    {
        cout << "  " << counter << " vowels makes the string not nice" << endl;
        is_nice = false;
    }
    
    regex_search(input, matches, rule_2_regex);
    if (matches.size() >= 1)
    {
        cout << "  An occurrence of a double letter keeps the same status" << endl;
    }
    else
    {
        cout << "  No occurrence of a double letter makes the string not nice" << endl;
        is_nice = false;
    }
    
    regex_search(input, matches, rule_3_regex);
    cout << " The first regex found " << matches.size() << " banned sequences" << endl;
    if (matches.size() == 0)
    {
        cout << "  No banned phrases keeps the same status" << endl;
    }
    else
    {
        cout << "  The banned phrase makes the string not nice." << endl;
        is_nice = false;
    }
    
    return is_nice;
}

string AocDay5::part1(string filename, vector<string> extra_args)
{
    vector<string> input = read_input(filename);
    long number_nice = 0;
    for (int i=0; i<input.size(); i++)
    {
        if (is_nice(input[i]))
        {
            number_nice++;
        }
    }
    
    ostringstream out;
    out << number_nice;
    return out.str();
}
