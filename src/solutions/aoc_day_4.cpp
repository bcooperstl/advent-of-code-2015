#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include <openssl/md5.h>

#include "aoc_day_4.h"
#include "file_utils.h"

using namespace std;

AocDay4::AocDay4():AocDay(4)
{
}

AocDay4::~AocDay4()
{
}

string AocDay4::read_input(string filename)
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

string AocDay4::part1(string filename, vector<string> extra_args)
{
    string data_str = read_input(filename);
    unsigned char result[MD5_DIGEST_LENGTH];
    int i = 0;
    char input[32];
    bool found = false;
    char data[32];
    
    strncpy(data, data_str.c_str(), 32);

    cout << "Checking for input " << data << endl;
    while (!found)
    {
        i++;
        if (i % 100000 == 0)
        {
            cout << " On iteration " << i << endl;
        }
        int length = snprintf(input, 32, "%s%d", data, i);
        MD5((unsigned char *)input, length, result);
        if ((result[0] == 0) && (result [1]) == 0 && ((result[2] & 0xf0) == 0))
        {
            cout << " Found matching hash target at i=" << i << endl;
            found = true;
        }
    }
    
    ostringstream out;
    out << i;
    return out.str();
}

string AocDay4::part2(string filename, vector<string> extra_args)
{
    string data_str = read_input(filename);
    unsigned char result[MD5_DIGEST_LENGTH];
    int i = 0;
    char input[32];
    bool found = false;
    char data[32];
    
    strncpy(data, data_str.c_str(), 32);

    cout << "Checking for input " << data << endl;
    while (!found)
    {
        i++;
        if (i % 100000 == 0)
        {
            cout << " On iteration " << i << endl;
        }
        int length = snprintf(input, 32, "%s%d", data, i);
        MD5((unsigned char *)input, length, result);
        if ((result[0] == 0) && (result [1]) == 0 && (result[2] == 0))
        {
            cout << " Found matching hash target at i=" << i << endl;
            found = true;
        }
    }
    
    ostringstream out;
    out << i;
    return out.str();
}
