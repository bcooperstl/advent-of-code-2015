#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_25.h"
#include "file_utils.h"

using namespace std;

AocDay25::AocDay25():AocDay(25)
{
}

AocDay25::~AocDay25()
{
}

void AocDay25::parse_input(string filename, int & row, int & column)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    
    // sample text: 
    
    // To continue, please consult the code grid in the manual.  Enter the code at row 2947, column 3029.
    // 0  1         2      3       4   5    6    7  8   9        10    11  12   13 14  15    16     17 
    // row is at position 15 and column is at position 17
    
    // Going to let the split string parser take care of removing commas and periods.
    char delims[3];
    delims[0]=' ';
    delims[1]=',';
    delims[2]='.';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    row = strtol(lines[0][15].c_str(), NULL, 10);
    column = strtol(lines[0][17].c_str(), NULL, 10);
    
    return;
}

unsigned long long AocDay25::find_value(int target_row, int target_column)
{
    int row = 1;
    int column = 1;
    
    unsigned long long value = 20151125ull;
    
    while (!((row == target_row) && (column == target_column)))
    {
        row--;
        column++;
        if (row == 0)
        {
            row = column;
            column = 1;
        }
        
        value = (value * 252533ull)%33554393ull;
        
        if (row == target_row || column == target_column)
        {
            cout << "Row " << row << " Column " << column << " is " << value << endl;
        }
    }
    
    return value;
}

string AocDay25::part1(string filename, vector<string> extra_args)
{
    int row;
    int column;
    
    parse_input(filename, row, column);
    
    cout << "Computing the value at row " << row << " column " << column << endl;
    
    unsigned long long value = find_value(row, column);
    
    ostringstream out;
    out << value;
    return out.str();
}

