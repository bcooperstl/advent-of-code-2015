#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_13.h"
#include "file_utils.h"

using namespace std;
using namespace Day13;

AocDay13::AocDay13():AocDay(13)
{
}

AocDay13::~AocDay13()
{
}

/* Input format:
Alice would gain 54 happiness units by sitting next to Bob.
Alice would lose 79 happiness units by sitting next to Carol.
*/

int AocDay13::parse_input(string filename, Person ** people)
{
    int count = 0;
    for (int i=0; i<MAX_PEOPLE; i++)
    {
        people[i] = NULL;
    }
    
    map<string, Person *> lookup;
    
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing the final . character so I don't have to do it later
    char delims[2];
    delims[0]=' ';
    delims[1]='.';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return 0;
    }

    for (int i=0; i<lines.size(); i++)
    {
        Person * person1;
        Person * person2;
        
        string person_name_1 = lines[i][0];
        map<string, Person *>::iterator person_iter_1 = lookup.find(person_name_1);
        if (person_iter_1 == lookup.end())
        {
            person1 = new Person();
            lookup[person_name_1] = person1;
            person1->name = person_name_1;
            cout << "Generating new Person for " << person_name_1 << endl;
            people[count] = person1;
            count++;
        }
        else
        {
            person1 = person_iter_1->second;
        }
        
        string person_name_2 = lines[i][10];
        map<string, Person *>::iterator person_iter_2 = lookup.find(person_name_2);
        if (person_iter_2 == lookup.end())
        {
            person2 = new Person();
            lookup[person_name_2] = person2;
            person2->name = person_name_2;
            cout << "Generating new Person for " << person_name_2 << endl;
            people[count] = person2;
            count++;
        }
        else
        {
            person2 = person_iter_2->second;
        }
        
        int factor = 0;
        if (lines[i][2] == "gain")
        {
            factor = 1;
        }
        else if (lines[i][2] == "lose")
        {
            factor = -1;
        }
        else
        {
            cerr << "Invalid gain/lose value " << lines[i][2] << endl;
        }
        int units = strtol(lines[i][3].c_str(), NULL, 10) * factor;
        cout << "Setting " << person_name_1 << " ---> " << person_name_2 << " happiness to be " << units << " units" << endl;
        person1->units[person2] = units;
    }
    
    return count;
}

int AocDay13::calculate_happiness(Person ** people, int num_people)
{
    //cout << "Calculating happiness " << endl;
    int happiness = 0;
    for (int i=0; i<num_people; i++)
    {
        int right_index = (i+1)%num_people;
        int left_index = (i-1+num_people)%num_people;
        //cout << "  " << people[i]->name << " next to " << people[right_index]->name << " allocates " << people[i]->units[people[right_index]] << " units" << endl;
        //cout << "  " << people[i]->name << " next to " << people[left_index]->name << " allocates " << people[i]->units[people[left_index]] << " units" << endl;
        
        happiness = happiness + people[i]->units[people[right_index]] + people[i]->units[people[left_index]];
    }
    //cout << " total happiness " << happiness << endl;
    return happiness;
}

void AocDay13::dump_cycle(Person ** people, int num_people)
{
    for (int i=0; i<num_people; i++)
    {
        cout << people[i]->name << " <-> ";
    }
    cout << people[0]->name;
}

// this will return the best length
// The initial k value should be one less than the number of people, since the first position is fixed.
void AocDay13::modified_heaps_algorithm_generate(Person ** people, int num_people, int k, int & best_case)
{
    //cout << " Called with num_people=" << num_people << " k = " << k << endl;
    if (k == 1)
    {
        if (compare_people_by_name(people[1], people[num_people-1]) < 0)
        {
            cout << "Running cycle: ";
            dump_cycle(people, num_people);
            cout << endl;

            int happiness = calculate_happiness(people, num_people);
            if (happiness > best_case)
            {
                cout << "New Best Happiness " << happiness << endl;
                best_case = happiness;
            
            }
        }
        else
        {
            cout << "Skipping cycle: ";
            dump_cycle(people, num_people);
            cout << endl;
        }
    }
    else
    {
        // Generate permutations with kth unaltered
        // Initially k == length(A)
        modified_heaps_algorithm_generate(people, num_people, k-1, best_case);

        // Generate permutations for kth swapped with each k-1 initial
        // This will work on an array going from positions 1 to k-1; Position 0 is fixed.
        for (int i=0; i<k-1; i++)
        {
            if (k%2 == 0)
            {
                Person * tmp = people[i+1];
                people[i+1] = people[k];
                people[k] = tmp;
            }
            else
            {
                Person * tmp = people[1];
                people[1] = people[k];
                people[k] = tmp;
            }
            modified_heaps_algorithm_generate(people, num_people, k-1, best_case);
        }
    }
}

int AocDay13::compare_people_by_name_func(const void *left, const void *right)
{
    return compare_people_by_name(*(Person **)left, *(Person **)right);
}

int AocDay13::compare_people_by_name(Person * left, Person * right)
{
    return strcmp(left->name.c_str(), right->name.c_str());
}

int AocDay13::find_best_arrangement(Person ** people, int num_people)
{
    int best_happiness = INT_MIN;

    qsort(people, num_people, sizeof(Person *), compare_people_by_name_func);

    modified_heaps_algorithm_generate(people, num_people, num_people-1, best_happiness);
    return best_happiness;
}

string AocDay13::part1(string filename, vector<string> extra_args)
{
    Person * people[MAX_PEOPLE];
    int num_people = parse_input(filename, people);
    
    ostringstream out;
    int best_happiness = find_best_arrangement(people, num_people);
    out << best_happiness;
    
    for (int i=0; i<num_people; i++)
    {
        delete people[i];
    }
    
    return out.str();
}

