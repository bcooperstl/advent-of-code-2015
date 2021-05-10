#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_18.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;

AocDay18::AocDay18():AocDay(18)
{
}

AocDay18::~AocDay18()
{
}

vector<string> AocDay18::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return raw_lines;
}

/*
The state a light should have next is based on its current state (on or off) plus the number of neighbors that are on:

    A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
    A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.
*/

void AocDay18::run_rules(Screen * screen)
{
    vector<pair<int, int>> turn_on;
    vector<pair<int, int>> turn_off;
    
    for (int x=screen->get_min_x(); x<=screen->get_max_x(); x++)
    {
        for (int y=screen->get_min_y(); y<=screen->get_max_y(); y++)
        {
            int num_neighbors_on = screen->num_matching_neighbors(x, y, LIGHT_ON, true); // true is to include diagnonals
            
            //A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
            if (screen->get(x,y) == LIGHT_ON)
            {
                if (num_neighbors_on != 2 && num_neighbors_on != 3)
                {
                    turn_off.push_back(pair<int,int>(x,y));
                }
            }
            
            // A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.
            if (screen->get(x,y) == LIGHT_OFF)
            {
                if (num_neighbors_on == 3)
                {
                    turn_on.push_back(pair<int,int>(x,y));
                }
            }
        }
    }
    screen->set_multi(turn_on, LIGHT_ON);
    screen->set_multi(turn_off, LIGHT_OFF);
    
    return;
}

string AocDay18::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 18 Part 2 requires 1 extra arguments: the number of iterations to run" << endl;
        return "";
    }

    int iterations = strtol(extra_args[0].c_str(), NULL, 10);

    Screen screen;
    
    vector<string> input = read_input(filename);
    
    screen.load(input);
    
    cout << "Start state:" << endl;
    screen.display();
    
    for (int i=1; i<=iterations; i++)
    {
        run_rules(&screen);
        cout << "After iteration " << i << ":" << endl;
        screen.display();
    }
    
    int on_count = screen.num_matching(LIGHT_ON);
    
    ostringstream out;
    out << on_count;
    return out.str();
}
