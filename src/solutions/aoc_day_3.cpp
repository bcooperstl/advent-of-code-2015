#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_3.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;

AocDay3::AocDay3():AocDay(3)
{
}

AocDay3::~AocDay3()
{
}

string AocDay3::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return "";
    }
    return raw_lines[0];
}

// Screen uses positive values as you move south and negative values as you move north, so opposite a traditional xy plane.
void AocDay3::calculate_edges(string directions, int & min_x, int & max_x, int & min_y, int & max_y)
{
    min_x=0;
    max_x=0;
    min_y=0;
    max_y=0;
    int pos_x=0;
    int pos_y=0;
    
    cout << "Direction:[" << directions << "]" << endl;
    for (int i=0; i<directions.size(); i++)
    {
        char ch = directions[i];
        switch (ch)
        {
            case INPUT_NORTH:
                pos_y -= 1;
                if (pos_y < min_y)
                {
                    min_y = pos_y;
                }
                break;
            case INPUT_SOUTH:
                pos_y += 1;
                if (pos_y > max_y)
                {
                    max_y = pos_y;
                }
                break;
            case INPUT_WEST:
                pos_x -= 1;
                if (pos_x < min_x)
                {
                    min_x = pos_x;
                }
                break;
            case INPUT_EAST:
                pos_x += 1;
                if (pos_x > max_x)
                {
                    max_x = pos_x;
                }
                break;
            default:
                cerr << "*****Invalid character " << ch << " provided in input!!" << endl;
        }
    }
    cout << "After going through " << directions.size() << " directions: min_x = " << min_x << " max_x = " << max_x << " min_y = " << min_y << " max_y = " << max_y << endl;
    return;
}

string AocDay3::part1(string filename, vector<string> extra_args)
{
    string directions = read_input(filename);
    int min_x, max_x, min_y, max_y;
    
    calculate_edges(directions, min_x, max_x, min_y, max_y);
    
    Screen * screen = new Screen(MAP_EMPTY, min_x, max_x, min_y, max_y);
    
    int pos_x=0;
    int pos_y=0;
    
    screen->set(pos_x, pos_y, MAP_VISITED);
    for (int i=0; i<directions.size(); i++)
    {
        char ch = directions[i];
        switch (ch)
        {
            case INPUT_NORTH:
                pos_y -= 1;
                break;
            case INPUT_SOUTH:
                pos_y += 1;
                break;
            case INPUT_WEST:
                pos_x -= 1;
                break;
            case INPUT_EAST:
                pos_x += 1;
                break;
            default:
                cerr << "*****Invalid character " << ch << " provided in input!!" << endl;
        }
        screen->set(pos_x, pos_y, MAP_VISITED);
    }
    
    cout << "After processing all " << directions.size() << " directions:" << endl;
    screen->display();
    int visited_count = screen->num_matching(MAP_VISITED);
    delete screen;
    
    ostringstream out;
    out << visited_count;
    return out.str();
}
