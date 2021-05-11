#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <set>
#include <map>

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

string AocDay19::part2(string filename, vector<string> extra_args)
{
    map<int, set<string>> history;
    map<string, vector<string>> rules;
    string target;
    
    parse_input(filename, rules, target);
    
    set<string> start;
    start.insert("e");
    
    history[0]=start;
    
    int steps = 1;
    while (1)
    {
        cout << "Working round " << steps << endl;
        set<string> inputs = history[steps-1];
        cout << " There are " << inputs.size() << " inputs to run through" << endl;
        
        if (inputs.size() == 0)
        {
            cerr << " NO INPUT TO RUN! " << endl;
            break;
        }
        
        
        
        set<string> combined;
        
        for (set<string>::iterator input_pos = inputs.begin(); input_pos != inputs.end(); ++input_pos)
        {
            string input = *input_pos;
            set<string> replacements = run_1_round(input, rules);
            //cout << "  Input " << input << " has " << replacements.size() << " replacements" << endl;
            for (set<string>::iterator repl_pos = replacements.begin(); repl_pos != replacements.end(); ++repl_pos)
            {
                string replacement = *repl_pos;
                if (combined.find(replacement) != combined.end())
                {
                    //cout << "   Replacement " << replacement << " already found at this level...skipping" << endl;
                    continue;
                }
                bool history_found = false;
                for (int prior=steps-1; prior>=1; prior--)
                {
                    if (history[prior].find(replacement) != history[prior].end())
                    {
                        //cout << "   Replacement " << replacement << " already found at prior level " << prior << "...skipping" << endl;
                        history_found = true;
                        break;
                    }
                }
                if (history_found)
                {
                    continue;
                }
                //cout << "   Replacement " << replacement << " is new....adding" << endl;
                combined.insert(replacement);
            }
        }
        if (combined.find(target) != combined.end())
        {
            cout << " TARGET FOUND" << endl;
            break;
        }
        history[steps]=combined;
        steps++;
    }
    
    ostringstream out;
    out << steps;
    return out.str();
}
