#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <algorithm>

#include "aoc_day_24.h"
#include "file_utils.h"

using namespace std;

AocDay24::AocDay24():AocDay(24)
{
}

AocDay24::~AocDay24()
{
}

vector<long> AocDay24::parse_input(string filename)
{
    vector<long> values;
    FileUtils fileutils;

    if (!fileutils.read_as_list_of_longs(filename, values))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return values;
    }

    sort(values.begin(), values.end(), greater<long>());
    
    return values;
}

long AocDay24::calculate_qr(long * weights, int num_weights)
{
    long ret = 1;
    for (int i=0; i<num_weights; i++)
    {
        ret *= weights[i];
    }
    return ret;
}

bool AocDay24::can_split_remainder_to_equal(long * values, int num_values, long target)
{
    /* 
    going to look for target assuming that the first value is included.
    since this is looking to split the values into two groups totalling target, it must be in one of the pairs
    */
    
    // going to use the bits to determine whether in or out of group 1
    long max_number = (1 << (num_values - 1)) - 1;
    for (int i=0; i<=max_number; i++)
    {
        long sum = values[0];
        
        /* 
        lets say we have 7 digits
        values[0] is always included
        will extract from values[1] to values[6], depending upon the number
        
        shift goes from 0 to 5 in this case
        
        */
        
        for (int shift = 0; shift < num_values - 1; shift++)
        {
            if (i & (1 << shift))
            {
                sum += values[num_values-shift-1]; // need the - 1 because we are zero-indexed
            }
        }
        if (sum == target)
        {
            return true;
        }
    }
    
    return false;
}

bool AocDay24::next_position(long * input, int input_size, int output_size, long * values, int * positions, long & current_sum)
{
    // sort of doing counting with the position array.
    // 
    
    for (int i = output_size - 1; i>=0; i--)
    {
        /*
        minimum at position i is i: 0 for the first position, 1 for the second postion ... i-1 for the ith position
        maximum at position i is input_size - (output_size - i). so
        
        if 10 input elements and 3 output elmeents
            i = 0 can go from 0 to 7
            i = 1 can go from 1 to 8
            i = 2 can go from 2 to 9
        
        */
        
        if (positions[i] == (input_size - (output_size - i)))
        {
            if (i == 0)
            {
                cout << " Should be done. Trying to carry beyond position 0" << endl;
                return false;
            }
            
            // need to carry. Set this one back to i--
            current_sum -= values[i];
            positions[i] = i;
            values[i] = input[positions[i]];
            current_sum += values[i];
        }
        else
        {
            // not carrying here. increment to the next value and stop
            current_sum -= values[i];
            positions[i]++;
            values[i] = input[positions[i]];
            current_sum += values[i];
            break;
        }
        
    }
    return true;
}

// input is sorted from largest to smallest
bool AocDay24::find_best_partition_3(long * input, int input_size, int output_size, long target, long * best_values, long & best_qr)
{
    cout << "Searching for best partition to make " << target << " out of " << output_size << " weights" << endl;
    
    long values[MAX_VALUES];
    int positions[MAX_VALUES];
    int num_used = 0;
    
    bool best_found = false;
    
    long shortcut_sum = 0;
    for (int i=0; i<output_size; i++)
    {
        shortcut_sum += input[i];
    }
    if (shortcut_sum < target)
    {
        cout << " Cannot make " << target << " out of the largest " << output_size << " weights. Shortcutting and returning false" << endl;
        return false;
    }
    
    long current_sum = 0;
    for (int i=0; i<output_size; i++)
    {
        values[i] = input[i];
        positions[i] = i;
        current_sum += input[i];
    }
    
    bool valid_position = true;
    
    while (valid_position)
    {
        //cout << " Checking ";
        //for (int i=0; i<output_size; i++)
        //{
        //    cout << " " << values[i];
        //}
        //cout << endl;

        if (current_sum == target)
        {
            //cout << " Potential match with";
            //for (int i=0; i<output_size; i++)
            //{
            //    cout << " " << values[i];
            //}
            //cout << endl;
            
            long remaining_values[MAX_VALUES];
            int remaining_pos = 0;
            for (int j=0; j<input_size; j++)
            {
                bool in_sum = false;
                for (int i=0; i<output_size; i++)
                {
                    if (positions[i] == j)
                    {
                        in_sum = true;
                        break;
                    }
                }
                if (!in_sum)
                {
                    remaining_values[remaining_pos] = input[j];
                    remaining_pos++;
                }
            }
               
            long qr = calculate_qr(values, output_size);
            if (qr < best_qr)
            {
                cout << " Potential best qr of " << qr << endl;
                if (can_split_remainder_to_equal(remaining_values, remaining_pos, target))
                {
                    cout << " Remaining values can be split. Good load possibility" << endl;
                    cout << " NEW BEST QR OF " << qr << " FOUND!" << endl;
                    best_qr = qr;
                    best_found = true;
                    for (int i=0; i<output_size; i++)
                    {
                        best_values[i] = values[i];
                    }
                }
                else
                {
                    cout << " Cannot split remaining values into two equal part" << endl;
                }
            }
        }
        
        valid_position = next_position(input, input_size, output_size, values, positions, current_sum);
    }
    
    return best_found;
}

// input is sorted from largest to smallest
bool AocDay24::find_best_partition_4(long * input, int input_size, int output_size, long target, long * best_values, long & best_qr)
{
    cout << "Searching for best partition to make " << target << " out of " << output_size << " weights" << endl;
    
    long values[MAX_VALUES];
    int positions[MAX_VALUES];
    int num_used = 0;
    
    bool best_found = false;
    
    long shortcut_sum = 0;
    for (int i=0; i<output_size; i++)
    {
        shortcut_sum += input[i];
    }
    if (shortcut_sum < target)
    {
        cout << " Cannot make " << target << " out of the largest " << output_size << " weights. Shortcutting and returning false" << endl;
        return false;
    }
    
    long current_sum = 0;
    for (int i=0; i<output_size; i++)
    {
        values[i] = input[i];
        positions[i] = i;
        current_sum += input[i];
    }
    
    bool valid_position = true;
    
    while (valid_position)
    {
        //cout << " Checking ";
        //for (int i=0; i<output_size; i++)
        //{
        //    cout << " " << values[i];
        //}
        //cout << endl;

        if (current_sum == target)
        {
            //cout << " Potential match with";
            //for (int i=0; i<output_size; i++)
            //{
            //    cout << " " << values[i];
            //}
            //cout << endl;
            
            long remaining_values[MAX_VALUES];
            int remaining_pos = 0;
            for (int j=0; j<input_size; j++)
            {
                bool in_sum = false;
                for (int i=0; i<output_size; i++)
                {
                    if (positions[i] == j)
                    {
                        in_sum = true;
                        break;
                    }
                }
                if (!in_sum)
                {
                    remaining_values[remaining_pos] = input[j];
                    remaining_pos++;
                }
            }
               
            long qr = calculate_qr(values, output_size);
            if (qr < best_qr)
            {
                cout << " Potential 4 best qr of " << qr << endl;
                int new_output_size = 1;
                bool found = false;
                long best_values_3[MAX_VALUES]; // dont care; need it as a placeholder
                long best_qr_3 = LONG_MAX; // dont care; need it as a placeholder
                while (new_output_size <= remaining_pos / 3)
                {
                    if (find_best_partition_3(remaining_values, remaining_pos, new_output_size, target, best_values_3, best_qr_3))
                    {
                        cout << " Remaining values can be split amongst 3 groups. Good load possibility" << endl;
                        cout << " NEW BEST 4 QR OF " << qr << " FOUND!" << endl;
                        best_qr = qr;
                        best_found = true;
                        for (int i=0; i<output_size; i++)
                        {
                            best_values[i] = values[i];
                        }
                        break;
                        found = true;
                    }
                    new_output_size++;
                }
                
                if (!found)
                {
                    cout << " Cannot split remaining values into three equal parts" << endl;
                }
            }
        }
        
        valid_position = next_position(input, input_size, output_size, values, positions, current_sum);
    }
    
    return best_found;
}


string AocDay24::part1(string filename, vector<string> extra_args)
{
    long all_values[MAX_VALUES];
    long best_values[MAX_VALUES];
    long best_qr = LONG_MAX;
    long sum = 0;
    long target;
    
    vector<long> weights = parse_input(filename);
    for (int i=0; i<weights.size(); i++)
    {
        all_values[i] = weights[i];
        sum += all_values[i];
    }
    
    target = sum / 3;
    
    
    int i = 1;
    while (!find_best_partition_3(all_values, weights.size(), i, target, best_values, best_qr))
    {
        i++;
    }

    ostringstream out;
    out << best_qr;
    return out.str();
}

string AocDay24::part2(string filename, vector<string> extra_args)
{
    long all_values[MAX_VALUES];
    long best_values[MAX_VALUES];
    long best_qr = LONG_MAX;
    long sum = 0;
    long target;
    
    vector<long> weights = parse_input(filename);
    for (int i=0; i<weights.size(); i++)
    {
        all_values[i] = weights[i];
        sum += all_values[i];
    }
    
    target = sum / 4;
    
    
    int i = 1;
    while (!find_best_partition_4(all_values, weights.size(), i, target, best_values, best_qr))
    {
        i++;
    }

    ostringstream out;
    out << best_qr;
    return out.str();
}
