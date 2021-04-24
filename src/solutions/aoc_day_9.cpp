#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_9.h"
#include "file_utils.h"

using namespace std;
using namespace Day9;

AocDay9::AocDay9():AocDay(9)
{
}

AocDay9::~AocDay9()
{
}

// London to Dublin = 464
int AocDay9::parse_input(string filename, City ** cities)
{
    int count = 0;
    for (int i=0; i<MAX_CITIES; i++)
    {
        cities[i] = NULL;
    }
    
    map<string, City *> lookup;
    
    FileUtils fileutils;
    vector<vector<string>> lines;
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }

    for (int i=0; i<lines.size(); i++)
    {
        City * city1;
        City * city2;
        
        string city_name_1 = lines[i][0];
        map<string, City *>::iterator city_iter_1 = lookup.find(city_name_1);
        if (city_iter_1 == lookup.end())
        {
            city1 = new City();
            lookup[city_name_1] = city1;
            city1->name = city_name_1;
            cout << "Generating new City for " << city_name_1 << endl;
            cities[count] = city1;
            count++;
        }
        else
        {
            city1 = city_iter_1->second;
        }
        
        string city_name_2 = lines[i][2];
        map<string, City *>::iterator city_iter_2 = lookup.find(city_name_2);
        if (city_iter_2 == lookup.end())
        {
            city2 = new City();
            lookup[city_name_2] = city2;
            city2->name = city_name_2;
            cout << "Generating new City for " << city_name_2 << endl;
            cities[count] = city2;
            count++;
        }
        else
        {
            city2 = city_iter_2->second;
        }
        
        int distance = strtol(lines[i][4].c_str(), NULL, 10);
        cout << "Setting " << city_name_1 << " <---> " << city_name_2 << " to be " << distance << endl;
        city1->distances[city2] = distance;
        city2->distances[city1] = distance;
    }
    
    return count;
}

int AocDay9::calculate_distance(City ** cities, int num_cities)
{
    //cout << "Calculating distance " << endl;
    int distance = 0;
    for (int i=0, j=1; i<num_cities-1; i++, j++)
    {
        //cout << "  " << cities[i]->name << " to " << cities[j]->name << " is " << cities[i]->distances[cities[j]] << endl;
        distance+=cities[i]->distances[cities[j]];
    }
    //cout << "  total distance " << distance << endl;
    return distance;
}

// this will return the best length
void AocDay9::heaps_algorithm_generate(City ** cities, int num_cities, int k, int & best_distance, int & worst_distance)
{
    if (k == 1)
    {
        int distance = calculate_distance(cities, num_cities);
        if ((best_distance == 0) || (distance < best_distance))
        {
            cout << "New Best Distance " << distance << endl;
            best_distance = distance;
        }
        if ((worst_distance == 0) || (distance > worst_distance))
        {
            cout << "New Worst Distance " << distance << endl;
            worst_distance = distance;
        }
    }
    else
    {
        // Generate permutations with kth unaltered
        // Initially k == length(A)
        heaps_algorithm_generate(cities, num_cities, k-1, best_distance, worst_distance);

        // Generate permutations for kth swapped with each k-1 initial
        for (int i=0; i<k-1; i++)
        {
            if (k%2 == 0)
            {
                City * tmp = cities[i];
                cities[i] = cities[k-1];
                cities[k-1] = tmp;
            }
            else
            {
                City * tmp = cities[0];
                cities[0] = cities[k-1];
                cities[k-1] = tmp;
            }
            heaps_algorithm_generate(cities, num_cities, k-1, best_distance, worst_distance);
        }
    }
}

int AocDay9::find_shortest_journey(City ** cities, int num_cities)
{
    int best_distance = 0;
    int worst_distance = 0;
    heaps_algorithm_generate(cities, num_cities, num_cities, best_distance, worst_distance);
    return best_distance;
}

int AocDay9::find_longest_journey(City ** cities, int num_cities)
{
    int best_distance = 0;
    int worst_distance = 0;
    heaps_algorithm_generate(cities, num_cities, num_cities, best_distance, worst_distance);
    return worst_distance;
}

string AocDay9::part1(string filename, vector<string> extra_args)
{
    City * cities[MAX_CITIES];
    int num_cities = parse_input(filename, cities);
    
    ostringstream out;
    int best_distance = find_shortest_journey(cities, num_cities);
    out << best_distance;
    
    for (int i=0; i<num_cities; i++)
    {
        delete cities[i];
    }
    
    return out.str();
}

string AocDay9::part2(string filename, vector<string> extra_args)
{
    City * cities[MAX_CITIES];
    int num_cities = parse_input(filename, cities);
    
    ostringstream out;
    int worst_distance = find_longest_journey(cities, num_cities);
    out << worst_distance;
    
    for (int i=0; i<num_cities; i++)
    {
        delete cities[i];
    }
    
    return out.str();
}
