#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_15.h"
#include "file_utils.h"

using namespace std;
using namespace Day15;

AocDay15::AocDay15():AocDay(15)
{
}

AocDay15::~AocDay15()
{
}

/* Input format:
Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3
*/

int AocDay15::parse_input(string filename, Ingredient ** ingredients)
{
    int count = 0;
    for (int i=0; i<MAX_INGREDIENTS; i++)
    {
        ingredients[i] = NULL;
    }
    
    map<string, Ingredient *> lookup;
    
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing colons and commas.
    char delims[3];
    delims[0]=' ';
    delims[1]=':';
    delims[2]=',';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 3, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return 0;
    }

    for (int i=0; i<lines.size(); i++)
    {
        ingredients[i] = new Ingredient();
        ingredients[i]->name = lines[i][0];
        ingredients[i]->capacity   = strtol(lines[i][2].c_str(), NULL, 10);
        ingredients[i]->durability = strtol(lines[i][4].c_str(), NULL, 10);
        ingredients[i]->flavor     = strtol(lines[i][6].c_str(), NULL, 10);
        ingredients[i]->texture    = strtol(lines[i][8].c_str(), NULL, 10);
        ingredients[i]->calories   = strtol(lines[i][10].c_str(), NULL, 10);
        
        cout << "Setting " << ingredients[i]->name << " with capacity " << ingredients[i]->capacity
             << " durability " << ingredients[i]->durability
             << " flavor " << ingredients[i]->flavor
             << " texture " <<ingredients[i]->texture
             << " calories " << ingredients[i]->calories << endl;
        
        count++;
    }
    
    cout << "count is " << count << endl;
    return count;
}

string AocDay15::part1(string filename, vector<string> extra_args)
{
    Ingredient * ingredients[MAX_INGREDIENTS];
    int num_ingredients = parse_input(filename, ingredients);
    
    long high_score = 0;
    
    if (num_ingredients == 2)
    {
        long quantities[2];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            quantities[1] = 100 - quantities[0];
            long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity;
            long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability;
            long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor;
            long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture;
            if (total_capacity < 0)
            {
                total_capacity = 0;
            }
            if (total_durability < 0)
            {
                total_durability = 0;
            }
            if (total_flavor < 0)
            {
                total_flavor = 0;
            }
            if (total_texture < 0)
            {
                total_texture = 0;
            }
            long score = total_capacity * total_durability * total_flavor * total_texture;

            if (score > high_score)
            {
                cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name << " and " << quantities[1] << " of " << ingredients[1]->name << endl;
                high_score = score;
            }
        }
    }
    else if (num_ingredients == 4)
    {
        long quantities[4];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            for (quantities[1] = 0; quantities[1]<=(100 - quantities[0]); quantities[1]++)
            {
                for (quantities[2] = 0; quantities[2]<=(100 - quantities[0] - quantities[1]); quantities[2]++)
                {
                    quantities[3] = 100 - quantities[0] - quantities[1] - quantities[2];
                    long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity   + quantities[2]*ingredients[2]->capacity   + quantities[3]*ingredients[3]->capacity;
                    long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability + quantities[2]*ingredients[2]->durability + quantities[3]*ingredients[3]->durability;
                    long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor     + quantities[2]*ingredients[2]->flavor     + quantities[3]*ingredients[3]->flavor;
                    long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture    + quantities[2]*ingredients[2]->texture    + quantities[3]*ingredients[3]->texture;
                    if (total_capacity < 0)
                    {
                        total_capacity = 0;
                    }
                    if (total_durability < 0)
                    {
                        total_durability = 0;
                    }
                    if (total_flavor < 0)
                    {
                        total_flavor = 0;
                    }
                    if (total_texture < 0)
                    {
                        total_texture = 0;
                    }
                    long score = total_capacity * total_durability * total_flavor * total_texture;
                    if (score > high_score)
                    {
                        cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name 
                        << " and " << quantities[1] << " of " << ingredients[1]->name 
                        << " and " << quantities[2] << " of " << ingredients[2]->name 
                        << " and " << quantities[3] << " of " << ingredients[3]->name << endl;
                        high_score = score;
                    }
                }
            }
        }        
    }
    else
    {
        cerr << "INVALID NUMBER OF INGREDIENTS " << num_ingredients << endl;
    }
    
    for (int i=0; i<num_ingredients; i++)
    {
        delete ingredients[i];
    }
    
    ostringstream out;
    out << high_score;
    return out.str();
}
