#ifndef __AOC_DAY_22__
#define __AOC_DAY_22__

#include <string>

#include "aoc_day.h"

#define MAX_TURNS 128

#define MAX_SPELLS 5

#define SPELL_MAGIC_MISSLE 0
#define SPELL_DRAIN 1
#define SPELL_SHEILD 2
#define SPELL_POISON 3
#define SPELL_RECHAHRGE 4
#define SPELL_NONE_OR_BOSS 99

#define PLAYER_START_MANA 500
#define PLAYER_START_ARMOR 0
#define PLAYER_START_HIT_POINTS 50
#define PLAYER_START_DAMAGE 0

#define GAME_NOT_OVER 0
#define GAME_OVER_PLAYER_WON 1
#define GAME_OVER_BOSS_WON 2

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
        int player_total_mana_spent;
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
        
        void clear_turn(GameStats * turn_stats, int current_turn);
        void init_turn(GameStats * turn_stats, int current_turn, bool apply_hard_mode_adjustment);
        void reinit_turn(GameStats * turn_stats, int current_turn, bool apply_hard_mode_adjustment);
        void apply_spell(GameStats * turn_stats, int current_turn, int spell_number);
        
        void boss_attack(GameStats * turn_stats, int current_turn);
        
        int is_game_over(GameStats * turn_stats, int current_turn);
        
        int find_player_win_least_mana(GameStats * turn_stats, bool is_hard_mode);
        
    public:
        AocDay22();
        ~AocDay22();
        
        string run_test_scenario_part1(int scenario_number);
        
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif

