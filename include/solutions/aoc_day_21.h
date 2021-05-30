#ifndef __AOC_DAY_21__
#define __AOC_DAY_21__

#include <string>

#include "aoc_day.h"

#define NUM_WEAPONS 5
#define NUM_ARMOR 5
#define NUM_RINGS 6 

namespace Day21
{
    struct Item
    {
        string name;
        int cost;
        int damage;
        int armor;
    };
    
    class Character
    {
        private:
            int m_start_hit_points;
            string m_name;
        public:
            Character(string name, int start_hit_points);
            int get_start_hit_points();
            string get_name();
            virtual int get_damage() = 0;
            virtual int get_armor() = 0;
    };
    
    class Enemy : public Character
    {
        private:
            int m_damage;
            int m_armor;
        public:
            Enemy(int start_hit_points, int damage, int armor);
            virtual int get_damage();
            virtual int get_armor();
    };
    
    class Player : public Character
    {
        private:
            Item * m_weapon;
            Item * m_armor;
            Item * m_rings[2];
        public:
            Player(int start_hit_points);
            virtual int get_damage();
            virtual int get_armor();
            void set_weapon(Item * weapon);
            void set_armor(Item * armor);
            void set_rings(Item * ring1, Item * ring2);
    };        
}

using namespace Day21;

class AocDay21 : public AocDay
{
    private:
        //Item m_null_item;
        Item m_weapons[NUM_WEAPONS+1]; // add 1 for no item
        Item m_armor[NUM_ARMOR+1]; // add 1 for no item
        Item m_rings[NUM_RINGS+1]; // add 1 for no item
        
        void init_items();
        
        void parse_input(string filename, int & enemy_hit_points, int & enemy_damage, int & enemy_armor);
    public:
        AocDay21();
        ~AocDay21();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
