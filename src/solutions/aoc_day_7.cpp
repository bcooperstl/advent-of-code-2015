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
    
    void Literal::fixup(map<string, Wire *> & lookup_map)
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
    
    void And::fixup(map<string, Wire *> & lookup_map)
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
    
    void Or::fixup(map<string, Wire *> & lookup_map)
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
    
    void Not::fixup(map<string, Wire *> & lookup_map)
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
        
        cout << m_name << ": Calculating value from NOT " << m_input_name << endl;
        m_calculated_value = ~m_input_wire->get_value();
        
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
    
    void RShift::fixup(map<string, Wire *> & lookup_map)
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
    
    void LShift::fixup(map<string, Wire *> & lookup_map)
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

    NoOp::NoOp(string name, string input_name):LogicGate(name)
    {
        m_input_name = input_name;
        m_input_wire = NULL;
    }
    
    NoOp::~NoOp()
    {
    }
    
    void NoOp::fixup(map<string, Wire *> & lookup_map)
    {
        m_input_wire = lookup_map[m_input_name];
    }

    uint16_t NoOp::get_value()
    {
        cout << m_name << ": Requesting value" << endl;
        if (m_evaluated == true)
        {
            cout << m_name << ": Returning stored value of " << m_calculated_value << endl;
            return m_calculated_value;
        }
        
        cout << m_name << ": Calculating value from NO-OP " << m_input_name << endl;
        m_calculated_value = m_input_wire->get_value();
        
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

/*
Samples...
456 -> y
x AND y -> d
x OR y -> e
x LSHIFT 2 -> f
y RSHIFT 2 -> g
NOT x -> h
x -> y // NoOp
*/

Wire * AocDay7::create_wire(vector<string> tokens)
{
    Wire * wire = NULL;
    if (tokens[1] == "AND")
    {
        wire = new And(tokens[4], tokens[0], tokens[2]);
    }
    else if (tokens[1] == "OR")
    {
        wire = new Or(tokens[4], tokens[0], tokens[2]);
    }
    else if (tokens[0] == "NOT")
    {
        wire = new Not(tokens[3], tokens[1]);
    }
    else if (tokens[1] == "RSHIFT")
    {
        wire = new RShift(tokens[4], tokens[0], strtol(tokens[2].c_str(), NULL, 10));
    }
    else if (tokens[1] == "LSHIFT")
    {
        wire = new LShift(tokens[4], tokens[0], strtol(tokens[2].c_str(), NULL, 10));
    }
    else if (tokens[1] == "->")
    {
        char test_char = tokens[0][0];
        if (test_char >= '0' && test_char <= '9') // number; create a literal
        {
            wire = new Literal(tokens[2], strtol(tokens[0].c_str(), NULL, 10));
        }
        else
        {
            wire = new NoOp(tokens[2], tokens[0]);
        }
    }
    else 
    {
        cerr << "Unable to create wire from: ";
        for (int i=0; i<tokens.size(); i++)
        {
            cerr << tokens[i] << ' ';
        }
        cerr << endl;
    }
    return wire;
}

string AocDay7::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 7 Part 1 requires 1 extra argument for the target wire" << endl;
        return "";
    }

    vector<vector<string>> lines = read_input(filename);
    map<string, Wire *> lookup;
    lookup["1"] = new Literal("1", 1); // kludge for the "1 AND x -> y" rules that need a 1 value
    for (int i=0; i<lines.size(); i++)
    {
        Wire * wire = create_wire(lines[i]);
        lookup[wire->get_name()] = wire;
        cout << "Storing value in wire " << wire->get_name() << endl;
    }
    
    for(map<string, Wire *>::iterator fixup_iter = lookup.begin(); fixup_iter != lookup.end(); ++fixup_iter)
    {
        fixup_iter->second->fixup(lookup);
    }
    
    cout << "Looking up value for " << extra_args[0] << endl;
    uint16_t value = lookup[extra_args[0]]->get_value();
    cout << "Retreived value " << value << endl;
    
    for(map<string, Wire *>::iterator del_iter = lookup.begin(); del_iter != lookup.end(); ++del_iter)
    {
        delete del_iter->second;
    }
    
    ostringstream out;
    out << value;
    return out.str();
}

string AocDay7::part2(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 2)
    {
        cerr << "Day 7 Part 2 requires 2 extra argument for 1) the target wire and 2) the replacement wire" << endl;
        return "";
    }

    vector<vector<string>> lines = read_input(filename);
    map<string, Wire *> lookup;
    lookup["1"] = new Literal("1", 1); // kludge for the "1 AND x -> y" rules that need a 1 value
    for (int i=0; i<lines.size(); i++)
    {
        Wire * wire = create_wire(lines[i]);
        lookup[wire->get_name()] = wire;
        cout << "Storing value in wire " << wire->get_name() << endl;
    }
    
    for(map<string, Wire *>::iterator fixup_iter = lookup.begin(); fixup_iter != lookup.end(); ++fixup_iter)
    {
        fixup_iter->second->fixup(lookup);
    }
    
    cout << "Looking up value for " << extra_args[0] << endl;
    uint16_t first_value = lookup[extra_args[0]]->get_value();
    cout << "Retreived value " << first_value << endl;
    
    for(map<string, Wire *>::iterator del_iter = lookup.begin(); del_iter != lookup.end(); ++del_iter)
    {
        delete del_iter->second;
    }
    
    lookup.clear();
    
    lookup["1"] = new Literal("1", 1); // kludge for the "1 AND x -> y" rules that need a 1 value
    lookup[extra_args[1]] = new Literal(extra_args[1], first_value);
    
    for (int i=0; i<lines.size(); i++)
    {
        Wire * wire = create_wire(lines[i]);
        if (wire->get_name() == extra_args[1])
        {
            cout << "Skipping re-creation for value " << wire->get_name();
            delete wire;
        }
        else
        {
            lookup[wire->get_name()] = wire;
            cout << "Storing value in wire " << wire->get_name() << endl;
        }
    }
    
    for(map<string, Wire *>::iterator fixup_iter = lookup.begin(); fixup_iter != lookup.end(); ++fixup_iter)
    {
        fixup_iter->second->fixup(lookup);
    }
    
    cout << "Looking up value for " << extra_args[0] << endl;
    uint16_t second_value = lookup[extra_args[0]]->get_value();
    cout << "Retreived value " << second_value << endl;
    
    ostringstream out;
    out << second_value;
    return out.str();
}
