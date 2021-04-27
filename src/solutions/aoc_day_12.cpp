#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

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

void AocDay12::remove_red(char * input, int input_length)
{
    int open_brace_positions[32]; // might need to make this larger
    int num_open_braces = 0;
    
    char tmp[32000];

    char * red_pos = strstr(input, ":\"red\"");

    while (red_pos != NULL)
    {
        cout << "First red found at position " << red_pos-input << endl;
        int open_to_find = 1;
        int close_to_find = 1;
        
        int open_search_pos = red_pos - input - 1; // start at the character before the first quote in "red"
        int close_search_pos = red_pos - input + 5; // start at the character after the second quote in "red"
        cout << "open search starting at " << open_search_pos << " " << input[open_search_pos] << endl;
        
        while ((open_search_pos >= 0) && (open_to_find > 0))
        {
            if (input[open_search_pos] == '}')
            {
                open_to_find++;
                cout << "found closing brace at position " << open_search_pos << " while searching for opening brace. Incrementing open_to_find to " << open_to_find << endl;
            }
            else if (input[open_search_pos] == '{')
            {
                open_to_find--;
                cout << "found opening brace at position " << open_search_pos << " while searching for opening brace. Decrementing open_to_find to " << open_to_find << endl;
            }
            open_search_pos--;
        }
        open_search_pos++; // undo the one extra
        
        if (open_to_find > 0)
        {
            cout << "****ERROR FINDING OPENING BRACE. CHECK YOUR LOGIC***" << endl;
        }
        
        cout << "close search starting at " << close_search_pos << " " << input[close_search_pos] << endl;
        while ((close_search_pos >= 0) && (close_to_find > 0))
        {
            if (input[close_search_pos] == '{')
            {
                close_to_find++;
                cout << "found opening brace at position " << close_search_pos << " while searching for closing brace. Incrementing close_to_find to " << close_to_find << endl;
            }
            else if (input[close_search_pos] == '}')
            {
                close_to_find--;
                cout << "found closing brace at position " << close_search_pos << " while searching for closing brace. Decrementing close_to_find to " << close_to_find << endl;
            }
            close_search_pos++;
        }
        close_search_pos--; // undo the one extra
        
        if (close_to_find > 0)
        {
            cout << "****ERROR FINDING CLOSING BRACE. CHECK YOUR LOGIC***" << endl;
        }
        
        cout << "going to remove from position " << open_search_pos + 1 << " " << input[open_search_pos + 1] << " to position " << close_search_pos - 1 << " " << input[close_search_pos - 1] << " to clear out this object" << endl;
        
        strncpy(tmp, input+close_search_pos, 32000);
        cout << tmp << endl;
        strncpy(input+open_search_pos+1, tmp, strlen(tmp)+1);
        cout << input << endl;
        red_pos = strstr(input, ":\"red\"");
    }
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

string AocDay12::part2(string filename, vector<string> extra_args)
{
    string json = read_input(filename);
    
    char message[32000];
    strncpy(message, json.c_str(), 32000);
    
    //cout << message << endl;
    
    remove_red(message, strlen(message));
    
    //cout << message << strlen(message) << endl;

    string new_message = string(message, strlen(message));
    vector<int> numbers = get_all_numbers(new_message);
    cout << "There were " << numbers.size() << " numbers found in the redless input json" << endl;
    
    int sum = 0;
    for (int i=0; i<numbers.size(); i++)
    {
        sum+=numbers[i];
    }
    
    ostringstream out;
    out << sum;
    return out.str();
}
