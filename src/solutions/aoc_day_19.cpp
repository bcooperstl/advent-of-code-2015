#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <set>
#include <map>
#include <algorithm>

#include "aoc_day_19.h"
#include "file_utils.h"

using namespace std;

AocDay19::AocDay19():AocDay(19)
{
}

AocDay19::~AocDay19()
{
}

/* Input format:
H => HO
H => OH
O => HH

HOH

*/

void AocDay19::parse_input(string filename, map<string, vector<string>> & rules, string & input)
{
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing colons and commas.
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    cout << "Parsing input file" << endl;

    for (int i=0; i<lines.size()-2; i++)
    {
        string base = lines[i][0];
        string replacement = lines[i][2];
        
        map<string, vector<string>>::iterator pos = rules.find(base);
        if (pos == rules.end())
        {
            cout << " Creating first entry for " << base << endl;
            rules[base]=vector<string>();
            pos = rules.find(base);
        }
        cout << " Appending " << replacement << " as a replacement for " << base << endl;
        pos->second.push_back(replacement);
    }
    
    input = lines[lines.size()-1][0];
    cout << " input string is " << input << endl;
    return;
}

set<string> AocDay19::run_1_round(string input, map<string, vector<string>> & rules)
{
    set<string> options;
    int length = input.length();
    int pos = 0;
    
    //cout << "Running rules on " << input << endl;
    while (pos < length)
    {
        int chars_used = 1;
        string target = input.substr(pos, 1);
        map<string, vector<string>>::iterator lookup = rules.find(target);
        if (lookup == rules.end())
        {
            chars_used = 2;
            target = input.substr(pos, 2);
            lookup = rules.find(target);
            if (lookup == rules.end())
            {
                //cerr << " Skipping character of " << input.substr(pos, 1) << " at position " << pos << " because it has no match and should remain" << endl;
                pos++;
                continue;
            }
        }
        
        for (vector<string>::iterator rules_iter = lookup->second.begin(); rules_iter != lookup->second.end(); ++rules_iter)
        {
            string dupe = input;
            string replacement = dupe.replace(pos, chars_used, *rules_iter);
            //cout << " Replacing " << target << " at " << pos << " with " << *rules_iter << " gives " << replacement << endl;
            options.insert(replacement);
        }
        pos+=chars_used;
    }

    return options;
}

string AocDay19::part1(string filename, vector<string> extra_args)
{
    map<string, vector<string>> rules;
    string input;
    
    parse_input(filename, rules, input);
    
    set<string> single_replacements = run_1_round(input, rules);
    
    ostringstream out;
    out << single_replacements.size();
    return out.str();
}

/*
 look through the reverse_rules. if any are found in input, apply them and then call recursively.
 short circuits to exit recursion:
    num_replacements >= best_replacements
    input matches a start string (only check if input length is less than or equal to max_start_string_length

*/
    
bool AocDay19::path_found_recursive(string input, vector<pair<string, string>> & reverse_rules, vector<string> & start_strings, int max_start_string_length, int num_replacements, int & best_replacements, set<string> & tried_and_false)
{
    //cout << "Processing Input " << input << " num_replacements " << num_replacements << endl;
    bool match_found = false;
    
    if (tried_and_false.find(input) != tried_and_false.end())
    {
        return false;
    }
    
    
    if (num_replacements >= best_replacements)
    {
        //cout << " Input " << input << " num_replacements " << num_replacements << " short circuiting because worse than best" << endl;
        return false; // already have something better
    }
    
    if (input.length() <= max_start_string_length)
    {
        for (int i=0; i<start_strings.size(); i++)
        {
            if (input == start_strings[i])
            {
                // we have a match at num_replacements + 1; the + 1 is for the replacement to e
                cout << " Input " << input << " num_replacements " << num_replacements << " match found after " << num_replacements + 1  << " replacements with " << start_strings[i] << endl;
                best_replacements = num_replacements + 1;
                return true;
            }
        }
    }
    
    for (int i=0; i<reverse_rules.size(); i++)
    {
        size_t pos = input.find(reverse_rules[i].first);
        if (pos != string::npos)
        {
            //cout << " Input " << input << " num_replacements " << num_replacements << " using match of " << reverse_rules[i].first << "->" << reverse_rules[i].second << " at " << pos << endl;
            string dupe = input;
            string replacement = dupe.replace(pos, reverse_rules[i].first.length(), reverse_rules[i].second);
            match_found = match_found || path_found_recursive(replacement, reverse_rules, start_strings, max_start_string_length, num_replacements+1, best_replacements, tried_and_false);
        }
    }
    
    if (!match_found)
    {
        //cout << "Adding " << input << " to tried and false" << endl;
        tried_and_false.insert(input);
    }

    return match_found;
}

bool AocDay19::sort_pair_length_order(const pair<string,string> &left, const pair<string,string> &right)
{
    int counters;
    int num_left=0;
    int num_right=0;
    
    // count occurrences of Rn, Y, and Ar
    
    for (int i=0; i<left.first.length(); i++)
    {
        if (i < left.first.length() -1)
        {
            if (left.first[i]=='R' && left.first[i+1]=='n') // Rn
            {
                num_left++;
            }
            if (left.first[i]=='A' && left.first[i+1]=='r') // Ar
            {
                num_left++;
            }
        }
        if (left.first[i]=='Y') // Y
        {
            num_left++;
        }
    }
    
    for (int i=0; i<right.first.length(); i++)
    {
        if (i < right.first.length() -1)
        {
            if (right.first[i]=='R' && right.first[i+1]=='n') // Rn
            {
                num_right++;
            }
            if (right.first[i]=='A' && right.first[i+1]=='r') // Ar
            {
                num_right++;
            }
        }
        if (right.first[i]=='Y') // Y
        {
            num_right++;
        }
    }
    
    if (num_left != num_right)
    {
        return num_left > num_right; // if the left one has more matches, it should be first.
    }
    
    if (left.first.length() == right.first.length())
    {
        return (left.first < right.first);
    }
    else
    {
        return (left.first.length() > right.first.length()); // want larger strings first to be greedy
    }
}

bool AocDay19::path_found_2(string input, vector<pair<string, string>> & reverse_rules, int & best_replacements)
{
    cout << "Processing Input " << input << endl;
    
    bool found = true;
    int counter = 0;
    while (input.length() > 1)
    {
        bool did_something = false;
        for (int i=0; i<reverse_rules.size(); i++)
        {
            size_t pos = input.find(reverse_rules[i].first);
            if (pos != string::npos)
            {
                cout << " Input " << input << " using match of " << reverse_rules[i].first << "->" << reverse_rules[i].second << " at " << pos << endl;
                input.replace(pos, reverse_rules[i].first.length(), reverse_rules[i].second);
                did_something = true;
                counter++;
            }
        }
        if (!did_something)
        {
            found = false;
            break;
        }
    }
    
    if (found)
    {
        best_replacements = counter;
    }
    return found;
}

string AocDay19::part2(string filename, vector<string> extra_args)
{
    map<string, vector<string>> rules;
    string target;
    
    vector<pair<string, string>> reverse_rules;
    vector<string> start_strings;
    int max_start_string_length = 0;
    
    set<string> tried_and_false;
    
    parse_input(filename, rules, target);
    
    int best_replacements = target.length();
    
    for (map<string, vector<string>>::iterator pos = rules.begin(); pos != rules.end(); ++pos)
    {
        string value = pos->first;
        for (vector<string>::iterator pos2 = pos->second.begin(); pos2 != pos->second.end(); ++pos2)
        {
            string repl = *pos2;
            cout << " Reverse rule from " << repl << " to " << value << " added" << endl;
            reverse_rules.push_back(make_pair(repl, value));
        }
    }
    
    sort(reverse_rules.begin(), reverse_rules.end(), sort_pair_length_order);
    cout << "Reverse rules after sort: " << endl;
    for (int i=0; i<reverse_rules.size(); i++)
    {
        cout << reverse_rules[i].first << " --> " << reverse_rules[i].second << endl;
    }
    
    bool ret = path_found_2(target, reverse_rules, best_replacements);
    while (!ret)
    {
        random_shuffle(reverse_rules.begin(), reverse_rules.end());
        ret = path_found_2(target, reverse_rules, best_replacements);
        cout << "Returned " << ret << " with best_replacements " << best_replacements << endl;
    }
    
    
    ostringstream out;
    out << best_replacements;
    return out.str();
}
