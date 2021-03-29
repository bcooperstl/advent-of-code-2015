#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_2.h"
#include "file_utils.h"

using namespace std;

AocDay2::AocDay2():AocDay(2)
{
}

AocDay2::~AocDay2()
{
}

vector<vector<long>> AocDay2::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> boxes;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_split_longs(filename, boxes, 'x', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return boxes;
}

long AocDay2::calculate_wrapping_paper(long length, long width, long height)
{
    long extra = 0;
    if (length >= width && length >= height)
    {
        extra = width * height;
    }
    else if (width >= length && width >= height)
    {
        extra = length * height;
    }
    else // height is largest
    {
        extra = length * width;
    }
    
    // the constant here is two (2) as a long, not 21 (twenty-one)
    return ((2l * length * width) + (2l * length * height) + (2l * width * height) + extra);
}

long AocDay2::calculate_ribbon(long length, long width, long height)
{
    long perimeter = 0;
    if (length >= width && length >= height)
    {
        perimeter = 2l * (width + height); // the constant here is two (2) as a long, not 21 (twenty-one)
    }
    else if (width >= length && width >= height)
    {
        perimeter = 2l * (length + height); // the constant here is two (2) as a long, not 21 (twenty-one)
    }
    else // height is largest
    {
        perimeter = 2l * (length + width); // the constant here is two (2) as a long, not 21 (twenty-one)
    }
    
    long bow = length * width * height;
    
    return perimeter + bow;
}

string AocDay2::part1(string filename, vector<string> extra_args)
{
    vector<vector<long>> boxes = read_input(filename);
    long total_paper = 0;
    for (int i=0; i<boxes.size(); i++)
    {
        long area = calculate_wrapping_paper(boxes[i][0], boxes[i][1], boxes[i][2]);
        total_paper += area;
        cout << " Box " << i << " is " << boxes[i][0] << "x" << boxes[i][1] << "x" << boxes[i][2] << " and requires " << area << " square feet of paper" << endl;
    }
    
    ostringstream out;
    out << total_paper;
    return out.str();
}

string AocDay2::part2(string filename, vector<string> extra_args)
{
    vector<vector<long>> boxes = read_input(filename);
    long total_ribbon = 0;
    for (int i=0; i<boxes.size(); i++)
    {
        long ribbon = calculate_ribbon(boxes[i][0], boxes[i][1], boxes[i][2]);
        total_ribbon += ribbon;
        cout << " Box " << i << " is " << boxes[i][0] << "x" << boxes[i][1] << "x" << boxes[i][2] << " and requires " << ribbon << " feet of ribbon" << endl;
    }
    
    ostringstream out;
    out << total_ribbon;
    return out.str();
}

