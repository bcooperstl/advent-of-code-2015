#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "aoc_day_20.h"
#include "file_utils.h"

using namespace std;

AocDay20::AocDay20():AocDay(20)
{
}

AocDay20::~AocDay20()
{
}

long AocDay20::read_input(string filename)
{
    FileUtils fileutils;
    vector<long> lines;
    if (!fileutils.read_as_list_of_longs(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return 0;
    }
    
    return lines[0];
}
        
string AocDay20::part1(string filename, vector<string> extra_args)
{
    long target = read_input(filename);
    
    long house = 1;
    while (1)
    {
        long sum = 0;
        long current = 1;
        while ((current * current) <= house)
        {
            if (house % current == 0)
            {
                sum += (current * 10);
                long dividend = house / current;
                if (dividend != current)
                {
                    sum += (dividend * 10);
                }
            }
            current++;
        }
        
        //cout << "House " << house << " got " << sum << " presents." << endl;
        
        if (sum >= target)
        {
            cout << "Found solution at " << house << " with " << sum << " presents." << endl;
            break;
        }
        
        house++;
    }
    
    ostringstream out;
    out << house;
    return out.str();
}

string AocDay20::part2(string filename, vector<string> extra_args)
{
    long target = read_input(filename);
    
    long house = 1;
    while (1)
    {
        long sum = 0;
        long elf = 1;
        while ((elf * elf) <= house)
        {
            if (house % elf == 0)
            {
                long dividend = house / elf;
                if (dividend <= 50) // within the first 50
                {
                    sum += (elf * 11);
                }
                if (dividend != elf)
                {
                    if (elf <= 50)
                    {
                        sum += (dividend * 11);
                    }
                }
            }
            elf++;
        }
        
        //cout << "House " << house << " got " << sum << " presents." << endl;
        
        if (sum >= target)
        {
            cout << "Found solution at " << house << " with " << sum << " presents." << endl;
            break;
        }
        
        house++;
    }
    
    ostringstream out;
    out << house;
    return out.str();
}
