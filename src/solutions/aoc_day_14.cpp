#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_14.h"
#include "file_utils.h"

using namespace std;
using namespace Day14;

AocDay14::AocDay14():AocDay(14)
{
}

AocDay14::~AocDay14()
{
}

/* Input format:
Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
*/

void AocDay14::parse_input(string filename, vector<Reindeer> & reindeer)
{
    FileUtils fileutils;
    vector<vector<string>> lines;

    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    for (int i=0; i<lines.size(); i++)
    {
        Reindeer current;
        
        current.name = lines[i][0];
        current.flight_speed = strtol(lines[i][3].c_str(), NULL, 10);
        current.flight_duration = strtol(lines[i][6].c_str(), NULL, 10);
        current.rest_duration = strtol(lines[i][13].c_str(), NULL, 10);
        
        cout << "Reindeer  " << current.name << " flies at " << current.flight_speed << " for " << current.flight_duration << " seconds then rests for " << current.rest_duration << " seconds" << endl;
        reindeer.push_back(current);
    }
    
    return;
}

int AocDay14::calculate_distance_for_time(Reindeer reindeer, int seconds)
{
    int distance = 0;
    int cycle_time = reindeer.flight_duration + reindeer.rest_duration;
    int full_cycles = seconds / cycle_time;
    int remaining_time = seconds % cycle_time;
    
    distance += (full_cycles * reindeer.flight_speed * reindeer.flight_duration);
    
    if (remaining_time > reindeer.flight_duration)
    {
        remaining_time = reindeer.flight_duration;
    }
    distance += (reindeer.flight_speed * remaining_time);
    
    cout << "Reindeer " << reindeer.name << " flies " << distance << " over " << seconds << " seconds" << endl;
    return distance;
}

string AocDay14::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 14 Part 1 requires 1 extra argument for the number of steps" << endl;
        return "";
    }

    int seconds = strtoul(extra_args[0].c_str(), NULL, 10);
    vector<Reindeer> reindeer;
    
    parse_input(filename, reindeer);
    
    int furthest_distance = 0;
    
    for (int i=0; i<reindeer.size(); i++)
    {
        int distance = calculate_distance_for_time(reindeer[i], seconds);
        if (distance > furthest_distance)
        {
            furthest_distance = distance;
        }
    }
    
    ostringstream out;
    out << furthest_distance;
    return out.str();
}
