#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "aoc_day_11.h"
#include "file_utils.h"

using namespace std;

AocDay11::AocDay11():AocDay(11)
{
}

AocDay11::~AocDay11()
{
}

string AocDay11::read_input(string filename)
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

// This function will ensure that any i o and l characters are removed from the input to generate the first next password.
void AocDay11::generate_first_next_password(char * input, int input_len)
{
    bool did_something = false;
    
    cout << "First advancement of " << input << " is ";
    
    for (int i=0; i<input_len; i++)
    {
        if (input[i] == 'i' || input[i] == 'l' || input[i] == 'o')
        {
            input[i] = input[i]+1; // skip to j, m, or p
            did_something = true;
        }
        
        if (did_something)
        {
            for (int j=i+1; j<input_len; j++)
            {
                input[j] = 'a';
            }
            break;
        }
    }
    
    if (!did_something) // no i l or o characters. fall back to normal routine
    {
        cout << " going to take place via the normal method" << endl;
        generate_next_password(input, input_len);
    }
    else
    {
        cout << input << endl;
    }
}

void AocDay11::generate_next_password(char * input, int input_len)
{
    char next[26]={'b', 'c', 'd', 'e', 'f', // abcde
                   'g', 'h', 'j', '\0', 'k', // fghij
                   'm', '\0', 'n', 'p', '\0', // klmno
                   'q', 'r', 's', 't', 'u', // pqrst
                   'v', 'w', 'x', 'y', 'z', // uvwxy
                   'a'};// z
    
    //cout << "After " << input << " is " ;
    
    if (input[input_len-1] != 'z') // easy case; just increment the last character
    {
        input[input_len-1] = next[input[input_len-1]-'a'];
    }
    else
    {
        for (int i=input_len-1; i>=0; i--)
        {
            if (input[i] == 'z')
            {
                input[i] = 'a';
            }
            else
            {
                input[i] = next[input[i]-'a'];
                break;
            }
        }
    }
    
    //cout << input << endl;
}

bool AocDay11::is_valid_password(char * input, int input_len)
{
    bool first_rule_met = false;
    bool third_rule_met = false;
    // not checking second rule - we take care of it while generating next password to not have i l or o characters
    
    // first rule - check for three increasing letters
    for (int i=0; i<input_len-2; i++)
    {
        if ((input[i+1] == (input[i] + 1)) && (input[i+2] == (input[i] + 2)))
        {
            first_rule_met = true;
        }
    }
    
    // third rule - two sets of matching letters
    int num_pairs = 0;
    for (int i=0; i<input_len-1; i++)
    {
        if (input[i+1] == input[i])
        {
            num_pairs++;
            i++; // skip the second letter, so aaa doesn't count as two pairs
        }
    }
    if (num_pairs >= 2)
    {
        third_rule_met = true;
    }
    return (first_rule_met && third_rule_met);
}
        
string AocDay11::part1(string filename, vector<string> extra_args)
{
    char input[MAX_PASSWORD_LEN+1];

    string input_str = read_input(filename);
    
    strncpy(input, input_str.c_str(), MAX_PASSWORD_LEN+1);
    int length = strlen(input);
    
    generate_first_next_password(input, length);
    
    while (!is_valid_password(input, length))
    {
        generate_next_password(input, length);
    }
    
    ostringstream out;
    out << input;
    return out.str();
}

