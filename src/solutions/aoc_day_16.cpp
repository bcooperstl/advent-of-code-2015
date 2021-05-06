#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_16.h"
#include "file_utils.h"

using namespace std;
using namespace Day16;

namespace Day16
{
    Sue::Sue(int number)
    {
        m_number = number;
        m_children = UNDEF_QUANTITY;
        m_cats = UNDEF_QUANTITY;
        m_samoyeds = UNDEF_QUANTITY;
        m_pomeranians = UNDEF_QUANTITY;
        m_akitas = UNDEF_QUANTITY;
        m_vizslas = UNDEF_QUANTITY;
        m_goldfish = UNDEF_QUANTITY;
        m_trees = UNDEF_QUANTITY;
        m_cars = UNDEF_QUANTITY;
        m_perfumes = UNDEF_QUANTITY;        
    }
    
    Sue::~Sue()
    {
    }
    
    int Sue::get_number()
    {
        return m_number;
    }
    
    void Sue::set_children(int value)
    {
        m_children = value;
    }
    
    void Sue::set_cats(int value)
    {
        m_cats = value;
    }
    
    void Sue::set_samoyeds(int value)
    {
        m_samoyeds = value;
    }
    
    void Sue::set_pomeranians(int value)
    {
        m_pomeranians = value;
    }
    
    void Sue::set_akitas(int value)
    {
        m_akitas = value;
    }
    
    void Sue::set_vizslas(int value)
    {
        m_vizslas = value;
    }
    
    void Sue::set_goldfish(int value)
    {
        m_goldfish = value;
    }
    
    void Sue::set_trees(int value)
    {
        m_trees = value;
    }
    
    void Sue::set_cars(int value)
    {
        m_cars = value;
    }
    
    void Sue::set_perfumes(int value)
    {
        m_perfumes = value;
    }
    
    void Sue::set_value(string field, int value)
    {
        if (field == "children")
        {
            set_children(value);
        }
        else if (field == "cats")
        {
            set_cats(value);
        }
        else if (field == "samoyeds")
        {
            set_samoyeds(value);
        }
        else if (field == "pomeranians")
        {
            set_pomeranians(value);
        }
        else if (field == "akitas")
        {
            set_akitas(value);
        }
        else if (field == "vizslas")
        {
            set_vizslas(value);
        }
        else if (field == "goldfish")
        {
            set_goldfish(value);
        }
        else if (field == "trees")
        {    
            set_trees(value);
        }
        else if (field == "cars")
        {
            set_cars(value);
        }
        else if (field == "perfumes")
        {
            set_perfumes(value);
        }
        else
        {
            cerr << " INVALID TARGET " << field << " for Sue " << m_number << endl;
        }
    }
    
    bool Sue::is_match(const Sue & other)
    {
        return ((other.m_children    == UNDEF_QUANTITY || other.m_children    == m_children) &&
                (other.m_cats        == UNDEF_QUANTITY || other.m_cats        == m_cats) &&
                (other.m_samoyeds    == UNDEF_QUANTITY || other.m_samoyeds    == m_samoyeds) &&
                (other.m_pomeranians == UNDEF_QUANTITY || other.m_pomeranians == m_pomeranians) &&
                (other.m_akitas      == UNDEF_QUANTITY || other.m_akitas      == m_akitas) &&
                (other.m_vizslas     == UNDEF_QUANTITY || other.m_vizslas     == m_vizslas) &&
                (other.m_goldfish    == UNDEF_QUANTITY || other.m_goldfish    == m_goldfish) &&
                (other.m_trees       == UNDEF_QUANTITY || other.m_trees       == m_trees) &&
                (other.m_cars        == UNDEF_QUANTITY || other.m_cars        == m_cars) &&
                (other.m_perfumes    == UNDEF_QUANTITY || other.m_perfumes    == m_perfumes));
    }
    
}


AocDay16::AocDay16():AocDay(16)
{
}

AocDay16::~AocDay16()
{
}

/* Input format:
Sue 1: children: 1, cars: 8, vizslas: 7
Sue 2: akitas: 10, perfumes: 10, children: 5

*/

void AocDay16::parse_input(string filename, vector<Sue> & possiblilities)
{
    possiblilities.clear();
    
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
        return;
    }

    for (int i=0; i<lines.size(); i++)
    {
        int number = strtol(lines[i][1].c_str(), NULL, 10);
        
        Sue sue(number);
        
        for (int pos=2; pos<lines[i].size(); pos+=2)
        {
            string field = lines[i][pos];
            int value = strtol(lines[i][pos+1].c_str(), NULL, 10);
            sue.set_value(field, value);
        }
        possiblilities.push_back(sue);
    }
    return;
}

Sue AocDay16::create_target_sue()
{
    Sue sue(0);
    sue.set_value("children",3);
    sue.set_value("cats",7);
    sue.set_value("samoyeds",2);
    sue.set_value("pomeranians",3);
    sue.set_value("akitas",0);
    sue.set_value("vizslas",0);
    sue.set_value("goldfish",5);
    sue.set_value("trees",3);
    sue.set_value("cars",2);
    sue.set_value("perfumes",1);

    return sue;
}

string AocDay16::part1(string filename, vector<string> extra_args)
{
    vector<Sue> possiblilities;
    parse_input(filename, possiblilities);
    
    int match_number = 0;
    
    Sue target = create_target_sue();
    
    for (int i=0; i<possiblilities.size(); i++)
    {
        if (target.is_match(possiblilities[i]))
        {
            cout << "Match found for sue " << possiblilities[i].get_number() << endl;
            match_number = possiblilities[i].get_number();
        }
    }
    
    ostringstream out;
    out << match_number;
    return out.str();
}
