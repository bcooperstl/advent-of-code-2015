#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_7.h"
#include "file_utils.h"

using namespace std;
using namespace Day7;

namespace Day7
{
    Wire::Wire(string name)
    {
        m_name = name;
    }
    
    Wire::~Wire()
    {
    }
    
    string Wire::get_name()
    {
        return m_name;
    }
    
    Literal::Literal(string name, uint16_t value):Wire(name)
    {
        m_literal_value = value;
    }
    
    uint16_t Literal::get_value()
    {
        return m_literal_value;
    }
    
    void Literal::fixup(map<string, Wire *> lookup_map)
    {
    }
    
    LogicGate::LogicGate(string name):Wire(name)
    {
        m_evaluated = false;
    }
    
    LogicGate::~LogicGate()
    {
    }
    
    bool LogicGate::is_evaluated()
    {
        return m_evaluated;
    }
    
    void LogicGate::set_evaluated(bool value)
    {
        m_evaluated = value;
    }
    
    And::And(string name, string left_name, string right_name):LogicGate(name)
    {
        m_left_name = left_name;
        m_right_name = right_name;
        m_left_wire = NULL;
        m_right_wire = NULL;
    }
    
    And::~And()
    {
    }
    
    void And::fixup(map<string, Wire *> lookup_map)
    {
        m_left_wire  = lookup_map[m_left_name];
        m_right_wire = lookup_map[m_right_name];
    }

    uint16_t And::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from " << m_left_name << " AND " << m_right_name << endl;
        m_calculated_value = m_left_wire->get_value() & m_right_wire->get_value();
        
        cout << m_name << ": Storing and returning value of " << m_calculated_value << endl;
        set_evaluated(true);
        return m_calculated_value;
    }

    Or::Or(string name, string left_name, string right_name):LogicGate(name)
    {
        m_left_name = left_name;
        m_right_name = right_name;
        m_left_wire = NULL;
        m_right_wire = NULL;
    }
    
    Or::~Or()
    {
    }
    
    void Or::fixup(map<string, Wire *> lookup_map)
    {
        m_left_wire  = lookup_map[m_left_name];
        m_right_wire = lookup_map[m_right_name];
    }

    uint16_t Or::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from " << m_left_name << " OR " << m_right_name << endl;
        m_calculated_value = m_left_wire->get_value() | m_right_wire->get_value();
        
        cout << m_name << ": Storing and returning value of " << m_calculated_value << endl;
        set_evaluated(true);
        return m_calculated_value;
    }

    Not::Not(string name, string input_name):LogicGate(name)
    {
        m_input_name = input_name;
        m_input_wire = NULL;
    }
    
    Not::~Not()
    {
    }
    
    void Not::fixup(map<string, Wire *> lookup_map)
    {
        m_input_wire = lookup_map[m_input_name];
    }

    uint16_t Not::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from NOT" << m_input_name << endl;
        m_calculated_value = !m_input_wire->get_value();
        
        cout << m_name << ": Storing and returning value of " << m_calculated_value << endl;
        set_evaluated(true);
        return m_calculated_value;
    }

    RShift::RShift(string name, string input_name, int shift_bits):LogicGate(name)
    {
        m_input_name = input_name;
        m_input_wire = NULL;
        m_shift_bits = shift_bits;
    }
    
    RShift::~RShift()
    {
    }
    
    void RShift::fixup(map<string, Wire *> lookup_map)
    {
        m_input_wire = lookup_map[m_input_name];
    }

    uint16_t RShift::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from " << m_input_name << " RSHIFT " << m_shift_bits << endl;
        m_calculated_value = m_input_wire->get_value() >> m_shift_bits;
        
        cout << m_name << ": Storing and returning value of " << m_calculated_value << endl;
        set_evaluated(true);
        return m_calculated_value;
    }
    
    LShift::LShift(string name, string input_name, int shift_bits):LogicGate(name)
    {
        m_input_name = input_name;
        m_input_wire = NULL;
        m_shift_bits = shift_bits;
    }
    
    LShift::~LShift()
    {
    }
    
    void LShift::fixup(map<string, Wire *> lookup_map)
    {
        m_input_wire = lookup_map[m_input_name];
    }

    uint16_t LShift::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from " << m_input_name << " LSHIFT " << m_shift_bits << endl;
        m_calculated_value = m_input_wire->get_value() << m_shift_bits;
        
        cout << m_name << ": Storing and returning value of " << m_calculated_value << endl;
        set_evaluated(true);
        return m_calculated_value;
    }
    
}

AocDay7::AocDay7():AocDay(7)
{
}

AocDay7::~AocDay7()
{
}

vector<vector<string>> AocDay7::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<string>> lines;
    if (!fileutils.read_as_list_of_split_strings(filename, lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    return lines;
}

string AocDay7::part1(string filename, vector<string> extra_args)
{
    vector<vector<string>> lines = read_input(filename);
    
    ostringstream out;
    out << "";
    return out.str();
}

