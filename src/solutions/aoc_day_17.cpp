#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_17.h"
#include "file_utils.h"

using namespace std;

AocDay17::AocDay17():AocDay(17)
{
}

AocDay17::~AocDay17()
{
}

void AocDay17::read_input(string filename, vector<long> & values)
{
    FileUtils fileutils;

    if (!fileutils.read_as_list_of_longs(filename, values))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }

    return;
}

int AocDay17::number_of_combinations_to_target(vector<long> & values, int position, int target)
{
    if (position >= values.size())
    {
        return 0; // this will read off end of the array if we try to read it.
    }
    
    long current_value = values[position];
    int subtotal = 0;
    
    /*
    calculate total with including the value at given position
    if current_value >  target, it can't be included
    if current value == target, then there is one combination with it being 
    if current value <  target, then use it; find the number of combinations starting at position + 1 matching remaining target
    */
    
    if (current_value > target)
    {
        //cout << "At position " << position << " searching for target " << target << " skipping current_value " << current_value << " because it is larger than target" << endl;
    }
    if (current_value == target)
    {
        //cout << "At position " << position << " searching for target " << target << " adding one because current_value " << current_value << " matches the target" << endl;
        subtotal += 1;
    }
    if (current_value < target)
    {
        //cout << "At position " << position << " searching for target " << target << " using current_value " << current_value << " and searching for next level" << endl;
        subtotal += number_of_combinations_to_target(values, position+1, target-current_value);
    }
    
    
    // total with skipping at given position
    subtotal += number_of_combinations_to_target(values, position+1, target);
    
    //cout << "At position " << position << " searching for target " << target << " returning " << subtotal << " total possibilities" << endl;
    return subtotal;
}

void AocDay17::find_min_used_for_target_find_min(vector<long> & values, int position, int target, int num_used, int & min)
{
    if (position >= values.size())
    {
        return;
    }
    
    long current_value = values[position];
    
    if (current_value > target)
    {
    }
    if (current_value == target)
    {
        if ((num_used + 1) < min)
        {
            min = num_used + 1;
            cout << "Setting minimum number to " << min << endl;
        }
    }
    if (current_value < target)
    {
        find_min_used_for_target_find_min(values, position+1, target-current_value, num_used+1, min);
    }
    
    find_min_used_for_target_find_min(values, position+1, target, num_used, min);
    
    return;
}

int AocDay17::number_of_combinations_to_target_matching_count(vector<long> & values, int position, int target_value, int num_used, int target_count)
{
    if (position >= values.size())
    {
        return 0; // this will read off end of the array if we try to read it.
    }
    
    long current_value = values[position];
    int subtotal = 0;
    
    /*
    calculate total with including the value at given position
    if current_value >  target, it can't be included
    if current value == target, then there is one combination with it being 
    if current value <  target, then use it; find the number of combinations starting at position + 1 matching remaining target
    */
    
    if (current_value > target_value)
    {
        //cout << "At position " << position << " searching for target " << target << " skipping current_value " << current_value << " because it is larger than target" << endl;
    }
    if (current_value == target_value)
    {
        //cout << "At position " << position << " searching for target " << target << " adding one because current_value " << current_value << " matches the target" << endl;
        if ((num_used + 1) == target_count)
        {
            subtotal += 1;
        }
    }
    if (current_value < target_value)
    {
        //cout << "At position " << position << " searching for target " << target << " using current_value " << current_value << " and searching for next level" << endl;
        subtotal += number_of_combinations_to_target_matching_count(values, position+1, target_value-current_value, num_used+1, target_count);
    }
    
    
    // total with skipping at given position
    subtotal += number_of_combinations_to_target_matching_count(values, position+1, target_value, num_used, target_count);
    
    //cout << "At position " << position << " searching for target " << target << " returning " << subtotal << " total possibilities" << endl;
    return subtotal;
}

string AocDay17::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 17 Part 1 requires 1 extra argument for thenumber of liters to find" << endl;
        return "";
    }

    int liters = strtol(extra_args[0].c_str(), NULL, 10);
    vector<long> values;
    
    read_input(filename, values);
    
    int combos = number_of_combinations_to_target(values, 0, liters);
    ostringstream out;
    out << combos;
    return out.str();
}

string AocDay17::part2(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 17 Part 2 requires 1 extra argument for thenumber of liters to find" << endl;
        return "";
    }

    int liters = strtol(extra_args[0].c_str(), NULL, 10);
    vector<long> values;
    
    read_input(filename, values);
    
    int min_used = values.size();
    
    find_min_used_for_target_find_min(values, 0, liters, 0, min_used);
    cout << "The minimum to use is " << min_used << endl;
    
    int combos = number_of_combinations_to_target_matching_count(values, 0, liters, 0, min_used);
    ostringstream out;
    out << combos;
    return out.str();
}
