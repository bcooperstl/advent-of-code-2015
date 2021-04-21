#ifndef __AOC_DAY_7__
#define __AOC_DAY_7__

#include "aoc_day.h"

#include <map>
#include <cstdint>

namespace Day7
{
    class Wire
    {
        protected:
            string m_name;
        public:
            Wire(string name);
            ~Wire();
            string get_name();
            virtual uint16_t get_value() = 0;
            virtual void fixup(map<string, Wire *> lookup_map) = 0;
    };
    
    class Literal : public Wire
    {
        protected:
            uint16_t m_literal_value;
        public:
            Literal(string name, uint16_t value);
            ~Literal();
            uint16_t get_value();
            void fixup(map<string, Wire *> lookup_map);
    };
    
    class LogicGate : public Wire
    {
        protected:
            bool m_evaluated;
            uint16_t m_calculated_value;
        public:
            LogicGate(string name);
            ~LogicGate();
            bool is_evaluated();
            void set_evaluated(bool value);
            virtual void fixup(map<string, Wire *> lookup_map) = 0; 
            virtual uint16_t get_value() = 0;
    };
    
    class And : public LogicGate
    {
        protected:
            
            Wire * m_left_wire;
            Wire * m_right_wire;
            string m_left_name;
            string m_right_name;
        public:
            And(string name, string left_name, string right_name);
            ~And();
            void fixup(map<string, Wire *> lookup_map);
            uint16_t get_value();
    };
            
    class Or : public LogicGate
    {
        protected:
            Wire * m_left_wire;
            Wire * m_right_wire;
            string m_left_name;
            string m_right_name;
        public:
            Or(string name, string left_name, string right_name);
            ~Or();
            void fixup(map<string, Wire *> lookup_map);
            uint16_t get_value();
    };

    class Not : public LogicGate
    {
        protected:
            Wire * m_input_wire;
            string m_input_name;
        public:
            Not(string name, string input_name);
            ~Not();
            void fixup(map<string, Wire *> lookup_map);
            uint16_t get_value();
    };
    
    class RShift : public LogicGate
    {
        protected:
            Wire * m_input_wire;
            string m_input_name;
            int m_shift_bits;
        public:
            RShift(string name, string input_name, int shift_bits);
            ~RShift();
            void fixup(map<string, Wire *> lookup_map);
            uint16_t get_value();
    };
    
    class LShift : public LogicGate
    {
        protected:
            Wire * m_input_wire;
            string m_input_name;
            int m_shift_bits;
        public:
            LShift(string name, string input_name, int shift_bits);
            ~LShift();
            void fixup(map<string, Wire *> lookup_map);
            uint16_t get_value();
    };
}

using namespace Day7;

class AocDay7 : public AocDay
{
    private:
        vector<vector<string>> read_input(string filename);
        Wire * create_wire(vector<string> line);
        
    public:
        AocDay7();
        ~AocDay7();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
