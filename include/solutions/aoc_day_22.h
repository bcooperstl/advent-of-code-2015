#ifndef __AOC_DAY_22__
#define __AOC_DAY_22__

#include <string>

#include "aoc_day.h"

#define MAX_TURNS 128

#define MAX_SPELLS 5

#define SPELL_MAGIC_MISSLE 0
#define SPELL_DRAIN 0
#define SPELL_SHEILD 2
#define SPELL_POISON 3
#define SPELL_RECHAHRGE 4
#define SPELL_NONE_OR_BOSS 99

#define PLAYER_START_MANA 500
#define PLAYER_START_ARMOR 0
#define PLAYER_START_HIT_POINTS 50
#define PLAYER_START_DAMAGE 0


namespace Day22
{
    struct GameStats
    {
        int turn_number;
        int player_hit_points;
        int player_armor;
        int player_mana;
        int player_damage;
        int boss_hit_points;
        int boss_damage;
        int last_spell_played;
    };
    
    struct Spell
    {
        string name;
        int cost;
        int instant_damage_dealt;
        int instant_healing_applied;
        int num_turns;
        int armor_boost;
        int start_turn_damage_dealt;
        int start_turn_mana_mined;
    };
}

using namespace Day22;

class AocDay22 : public AocDay
{
    private:
        Spell m_spells[MAX_SPELLS];
        
        void parse_input(string filename, int & enemy_hit_points, int & enemy_damage);
        
        void init_spells();

        void setup_turn_0(GameStats * turn_stats, int player_hit_points, int player_armor, int player_mana, int player_damage, int enemy_hit_points, int enemy_damage);
        
        bool can_cast_spell(GameStats * turn_stats, int current_turn, int spell_number);
        
        void init_turn(GameStats * turn_stats, int current_turn);
        void apply_spell(GameStats * turn_stats, int current_turn, int spell_number);
        
        //bool battle(Player * player, Enemy * enemy);
    public:
        AocDay22();
        ~AocDay22();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
