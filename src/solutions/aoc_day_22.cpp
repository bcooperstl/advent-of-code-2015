#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_22.h"
#include "file_utils.h"

using namespace std;
using namespace Day22;

AocDay22::AocDay22():AocDay(22)
{
    init_spells();
}

AocDay22::~AocDay22()
{
}

void AocDay22::init_spells()
{
    m_spells[0].name = "Magic Missle";
    m_spells[0].cost = 53;
    m_spells[0].instant_damage_dealt = 4;
    m_spells[0].instant_healing_applied = 0;
    m_spells[0].num_turns = 0;
    m_spells[0].armor_boost = 0;
    m_spells[0].start_turn_damage_dealt = 0;
    m_spells[0].start_turn_mana_mined = 0;
    
    m_spells[1].name = "Drain";
    m_spells[1].cost = 73;
    m_spells[1].instant_damage_dealt = 2;
    m_spells[1].instant_healing_applied = 2;
    m_spells[1].num_turns = 0;
    m_spells[1].armor_boost = 0;
    m_spells[1].start_turn_damage_dealt = 0;
    m_spells[1].start_turn_mana_mined = 0;

    m_spells[2].name = "Shield";
    m_spells[2].cost = 113;
    m_spells[2].instant_damage_dealt = 0;
    m_spells[2].instant_healing_applied = 0;
    m_spells[2].num_turns = 6;
    m_spells[2].armor_boost = 7;
    m_spells[2].start_turn_damage_dealt = 0;
    m_spells[2].start_turn_mana_mined = 0;

    m_spells[3].name = "Poison";
    m_spells[3].cost = 173;
    m_spells[3].instant_damage_dealt = 0;
    m_spells[3].instant_healing_applied = 0;
    m_spells[3].num_turns = 6;
    m_spells[3].armor_boost = 0;
    m_spells[3].start_turn_damage_dealt = 3;
    m_spells[3].start_turn_mana_mined = 0;

    m_spells[4].name = "Recharge";
    m_spells[4].cost = 229;
    m_spells[4].instant_damage_dealt = 0;
    m_spells[4].instant_healing_applied = 0;
    m_spells[4].num_turns = 5;
    m_spells[4].armor_boost = 0;
    m_spells[4].start_turn_damage_dealt = 0;
    m_spells[4].start_turn_mana_mined = 101;
}

/* Input format:
Hit Points: 109
Damage: 8
Armor: 2
*/

void AocDay22::parse_input(string filename, int & enemy_hit_points, int & enemy_damage)
{
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing colons.
    char delims[2];
    delims[0]=' ';
    delims[1]=':';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 2, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }

    enemy_hit_points = strtol(lines[0][2].c_str(), NULL, 10);
    enemy_damage = strtol(lines[1][1].c_str(), NULL, 10);
    
    return;
}

bool AocDay22::can_cast_spell(GameStats * turn_stats, int current_turn, int spell_number)
{
    // first check that we have enough mana to call the spell
    if (turn_stats[current_turn].player_mana < m_spells[spell_number].cost)
    {
        cout << "Unable to call " << m_spells[spell_number].name 
             << " because its cost is " << m_spells[spell_number].cost 
             << " and player only has " << turn_stats[current_turn].player_mana << endl;
        return false;
    }
    
    // if the spell does not have a num_turns value, we can turn true here. it can be called any time
    if (m_spells[spell_number].num_turns == 0)
    {
        cout << "Able to call " << m_spells[spell_number].name << " which does not have a number of turns" << endl;
        return true;
    }
    
    for (int turn = current_turn-1; ((turn >= 0) && (turn >= (current_turn - m_spells[spell_number].num_turns ))); turn--)
    {
        if (turn_stats[turn].last_spell_played == spell_number)
        {
            cout << "Unable to call " << m_spells[spell_number].name 
                << " because its num_turns is " << m_spells[spell_number].num_turns 
                << " and it was played on turn " << turn << " with current turn " << current_turn << endl;
            return false;
        }
    }
    
    cout << "Able to call " << m_spells[spell_number].name 
         << " because it was not played in the prior " << m_spells[spell_number].num_turns 
         << " with current turn " << current_turn << endl;
    return true;
}

void AocDay22::setup_turn_0(GameStats * turn_stats, int player_hit_points, int player_armor, int player_mana, int player_damage, int enemy_hit_points, int enemy_damage)
{
    turn_stats[0].turn_number = 0;
    turn_stats[0].player_hit_points = player_hit_points;
    turn_stats[0].player_armor = player_armor;
    turn_stats[0].player_mana = player_mana;
    turn_stats[0].player_damage = player_damage;
    turn_stats[0].boss_hit_points = enemy_hit_points;
    turn_stats[0].boss_damage = enemy_damage;
    turn_stats[0].last_spell_played = SPELL_NONE_OR_BOSS;
    turn_stats[0].player_total_mana_spent = 0;
}

// done the first time a turn is initialized. Will mark that no spell has been played
void AocDay22::init_turn(GameStats * turn_stats, int current_turn)
{
    reinit_turn(turn_stats, current_turn);
    turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
}

// done to reset to play the next spell. does not reset teh last_spell_played value
void AocDay22::reinit_turn(GameStats * turn_stats, int current_turn)
{
    int prior_turn = current_turn-1;
    cout << "Initializing turn " << current_turn << endl;
    turn_stats[current_turn].turn_number = current_turn;
    turn_stats[current_turn].player_hit_points = turn_stats[prior_turn].player_hit_points;
    turn_stats[current_turn].player_armor = turn_stats[prior_turn].player_armor;
    turn_stats[current_turn].player_mana = turn_stats[prior_turn].player_mana;
    turn_stats[current_turn].player_damage = turn_stats[prior_turn].player_damage;
    turn_stats[current_turn].boss_hit_points = turn_stats[prior_turn].boss_hit_points;
    turn_stats[current_turn].boss_damage = turn_stats[prior_turn].boss_damage;
    //turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
    turn_stats[current_turn].player_total_mana_spent = turn_stats[prior_turn].player_total_mana_spent;
    
    // loop over the spells to see what effects we need to apply or undo
    for (int i=0; i<MAX_SPELLS; i++)
    {
        if (m_spells[i].armor_boost > 0) // check to see if we need to unapply the armor boost.
        {
            int target_turn = current_turn - m_spells[i].num_turns;
            if (target_turn > 0)
            {
                if (turn_stats[target_turn].last_spell_played == i)
                {
                    cout << " Armor boost from turn " << target_turn 
                         << " has expired. Removing the value of " << m_spells[i].armor_boost 
                         << " from spell " << m_spells[i].name << endl;
                    turn_stats[current_turn].player_armor -= m_spells[i].armor_boost;
                }
            }
        }

        if (m_spells[i].start_turn_damage_dealt > 0)
        {
            for (int turn = current_turn-1; ((turn >= 0) && (turn >= (current_turn - m_spells[i].num_turns ))); turn--)
            {
                if (turn_stats[turn].last_spell_played == i)
                {
                    cout << " Dealing start of turn damage of " << m_spells[i].start_turn_damage_dealt
                         << " from spell " << m_spells[i].name 
                         << " called on turn " << turn << endl;
                    turn_stats[current_turn].boss_hit_points -= m_spells[i].start_turn_damage_dealt;
                    break;
                }
            }
        }

        if (m_spells[i].start_turn_mana_mined > 0)
        {
            for (int turn = current_turn-1; ((turn >= 0) && (turn >= (current_turn - m_spells[i].num_turns ))); turn--)
            {
                if (turn_stats[turn].last_spell_played == i)
                {
                    cout << " Mining mana to increase by " << m_spells[i].start_turn_mana_mined
                         << " from spell " << m_spells[i].name 
                         << " called on turn " << turn << endl;
                    turn_stats[current_turn].player_mana += m_spells[i].start_turn_mana_mined;
                    break;
                }
            }
        }
    }
    return;
}

void AocDay22::apply_spell(GameStats * turn_stats, int current_turn, int spell_number)
{
    int prior_turn = current_turn-1;
    cout << "Applying spell " << m_spells[spell_number].name << " on turn " << current_turn << endl;
    turn_stats[current_turn].player_mana -= m_spells[spell_number].cost;
    cout << " Decrementing player mana by " << m_spells[spell_number].cost << " to go to " << turn_stats[current_turn].player_mana << endl;
    turn_stats[current_turn].player_total_mana_spent += m_spells[spell_number].cost;
    
    if (m_spells[spell_number].armor_boost > 0)
    {
        turn_stats[current_turn].player_armor += m_spells[spell_number].armor_boost;
        cout << " Applying " << m_spells[spell_number].armor_boost 
             << " armor boost to the player, resulting in armor incrementing to " <<  turn_stats[current_turn].player_armor << endl;
    }
    
    if (m_spells[spell_number].instant_damage_dealt > 0)
    {
        turn_stats[current_turn].boss_hit_points -= m_spells[spell_number].instant_damage_dealt;
        cout << " Dealing " << m_spells[spell_number].instant_damage_dealt 
             << " damage to boss resulting in their hit points at " << turn_stats[current_turn].boss_hit_points << endl;
    }
    
    if (m_spells[spell_number].instant_healing_applied > 0)
    {
        turn_stats[current_turn].player_hit_points += m_spells[spell_number].instant_healing_applied;
        cout << " Healing " << m_spells[spell_number].instant_healing_applied 
             << " hit points to bring player to " << turn_stats[current_turn].player_hit_points << endl;
    }
    
    turn_stats[current_turn].last_spell_played = spell_number;
    
    return;
}

void AocDay22::boss_attack(GameStats * turn_stats, int current_turn)
{
    cout << "Boss attacking on turn " << current_turn << endl;
    
    int player_hit_points = turn_stats[current_turn].player_hit_points;
    int boss_hit_points = turn_stats[current_turn].boss_hit_points;
    
    cout << " Player starts with " << player_hit_points << " hit points" << endl;
    cout << " Boss starts with " << boss_hit_points << " hit points" << endl;
    
    int boss_damage_dealt = turn_stats[current_turn].boss_damage - turn_stats[current_turn].player_armor;
    
    if (boss_damage_dealt < 1)
    {
        boss_damage_dealt = 1;
    }
    
    cout << " The boss does " << boss_damage_dealt 
         << " damage based on having " << turn_stats[current_turn].boss_damage
         << " damage against " << turn_stats[current_turn].player_armor
         << " player armor" << endl;
    
    turn_stats[current_turn].player_hit_points -= boss_damage_dealt;
}

int AocDay22::is_game_over(GameStats * turn_stats, int current_turn)
{
    if (turn_stats[current_turn].boss_hit_points > 0)
    {
        if (turn_stats[current_turn].player_hit_points > 0)
        {
            return GAME_NOT_OVER;
        }
        else
        {
            return GAME_OVER_BOSS_WON;
        }
    }
    else
    {
        if (turn_stats[current_turn].player_hit_points > 0)
        {
            return GAME_OVER_PLAYER_WON;
        }
        else
        {
            cerr << "INVALID GAME SCENARIO -BOTH LOSS" << endl;
            return GAME_NOT_OVER;
        }
    }
}

string AocDay22::run_test_scenario_part1(int scenario_number)
{
    int num_differences = 0;
    
    cout << "Running test scenario " << scenario_number << endl;
    
    GameStats turn_stats[MAX_TURNS + 1]; // add the +1 for the initial setup in turn 0;
    memset(turn_stats, 0, sizeof(struct GameStats) * (MAX_TURNS + 1));
    
    if (scenario_number == 1)
    {
        // For example, suppose the player has 10 hit points and 250 mana, and that the boss has 13 hit points and 8 damage:
        setup_turn_0(turn_stats, 10, 0, 250, 0, 13, 8);
        
        init_turn(turn_stats, 1);
        
        if (is_game_over(turn_stats, 1) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        apply_spell(turn_stats, 1, SPELL_POISON);
        if (turn_stats[1].last_spell_played != SPELL_POISON)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[1].player_mana != 77)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[1].player_total_mana_spent != 173)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 1) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        init_turn(turn_stats, 2);
        if (turn_stats[2].boss_hit_points != 10)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 2) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        boss_attack(turn_stats, 2);
        if (turn_stats[2].player_hit_points != 2)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 2) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        init_turn(turn_stats, 3);
        if (turn_stats[3].boss_hit_points != 7)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 3) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        apply_spell(turn_stats, 3, SPELL_MAGIC_MISSLE);
        if (turn_stats[3].last_spell_played != SPELL_MAGIC_MISSLE)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[3].player_mana != 24)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[3].player_total_mana_spent != 226)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[3].boss_hit_points != 3)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 3) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        init_turn(turn_stats, 4);
        if (turn_stats[4].boss_hit_points != 0)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, 4) != GAME_OVER_PLAYER_WON)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        cout << "Scenario 1 completed" << endl;
    }
    else if (scenario_number == 2)
    {
        // For example, suppose the player has 10 hit points and 250 mana, and that the boss has 14 hit points and 8 damage:
        setup_turn_0(turn_stats, 10, 0, 250, 0, 14, 8);
        
        int current_turn = 1;
        init_turn(turn_stats, current_turn);
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        apply_spell(turn_stats, current_turn, SPELL_RECHAHRGE);
        if (turn_stats[current_turn].last_spell_played != SPELL_RECHAHRGE)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_mana != 21)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_total_mana_spent != 229)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 2;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 122)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        boss_attack(turn_stats, current_turn);
        if (turn_stats[current_turn].player_hit_points != 2)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 3;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 223)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        apply_spell(turn_stats, current_turn, SPELL_SHEILD);
        if (turn_stats[current_turn].last_spell_played != SPELL_SHEILD)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_mana != 110)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_total_mana_spent != 342)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        current_turn = 4;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 211)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 7)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        boss_attack(turn_stats, current_turn);
        if (turn_stats[current_turn].player_hit_points != 1)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 5;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 312)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 7)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        apply_spell(turn_stats, current_turn, SPELL_DRAIN);
        if (turn_stats[current_turn].last_spell_played != SPELL_DRAIN)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_mana != 239)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_total_mana_spent != 415)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_hit_points != 3)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].boss_hit_points != 12)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        current_turn = 6;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 340)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 7)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        boss_attack(turn_stats, current_turn);
        if (turn_stats[current_turn].player_hit_points != 2)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 7;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 340)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 7)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        apply_spell(turn_stats, current_turn, SPELL_POISON);
        if (turn_stats[current_turn].last_spell_played != SPELL_POISON)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_mana != 167)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_total_mana_spent != 588)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 8;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 167)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 7)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].boss_hit_points != 9)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        boss_attack(turn_stats, current_turn);
        if (turn_stats[current_turn].player_hit_points != 1)
        {
            cerr << " player_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 9;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 167)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 0)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].boss_hit_points != 6)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }

        apply_spell(turn_stats, current_turn, SPELL_MAGIC_MISSLE);
        if (turn_stats[current_turn].last_spell_played != SPELL_MAGIC_MISSLE)
        {
            cerr << " last_spell_played MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_mana != 114)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_total_mana_spent != 641)
        {
            cerr << " player_total_mana_spent MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].boss_hit_points != 2)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_NOT_OVER)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        
        current_turn = 10;
        init_turn(turn_stats, current_turn);
        if (turn_stats[current_turn].player_mana != 114)
        {
            cerr << " player_mana MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].player_armor != 0)
        {
            cerr << " player_armor MISMATCH" << endl;
            num_differences++;
        }
        if (turn_stats[current_turn].boss_hit_points != -1)
        {
            cerr << " boss_hit_points MISMATCH" << endl;
            num_differences++;
        }
        if (is_game_over(turn_stats, current_turn) != GAME_OVER_PLAYER_WON)
        {
            cerr << " is_game_over MISMATCH" << endl;
            num_differences++;
        }
        cout << "Scenario 2 completed" << endl;
    }
    else
    {
        cerr << "INVALID SCENARIO NUMBER " << scenario_number << endl;
        return "";
    }

    ostringstream out;
    out << num_differences;
    return out.str();
}

string AocDay22::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() == 1)
    {
        return run_test_scenario_part1(strtol(extra_args[0].c_str(), NULL, 10));
    }

    GameStats turn_stats[MAX_TURNS + 1]; // add the +1 for the initial setup in turn 0;
    memset(turn_stats, 0, sizeof(struct GameStats) * (MAX_TURNS + 1));
    
    int enemy_hit_points, enemy_damage;
    
    int least_mana_used = INT_MAX;
    
    parse_input(filename, enemy_hit_points, enemy_damage);
    
    setup_turn_0(turn_stats, PLAYER_START_HIT_POINTS, PLAYER_START_ARMOR, PLAYER_START_MANA, PLAYER_START_DAMAGE, enemy_hit_points, enemy_damage);
    
    ostringstream out;
    out << least_mana_used;
    return out.str();
}
