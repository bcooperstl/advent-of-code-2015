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

void AocDay22::init_turn(GameStats * turn_stats, int current_turn)
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
    turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
    
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
    return;
}


/*
// Returns true if the player wins or false if the enemy wins
bool AocDay22::battle(Player * player, Enemy * enemy)
{
    cout << "Battling with player having ";
    player->display_equipment();
    cout << endl;
    
    int player_hit_points = player->get_start_hit_points();
    int enemy_hit_points = enemy->get_start_hit_points();
    
    cout << "Player starts with " << player_hit_points << " hit points" << endl;
    cout << "Enemy starts with " << enemy_hit_points << " hit points" << endl;
    
    int player_damage_dealt = player->get_damage() - enemy->get_armor();
    int enemy_damage_dealt = enemy->get_damage() - player->get_armor();
    
    if (player_damage_dealt < 1)
    {
        player_damage_dealt = 1;
    }
    
    if (enemy_damage_dealt < 1)
    {
        enemy_damage_dealt = 1;
    }
    
    cout << "Player deals " << player_damage_dealt << " each round" << endl;
    cout << "Enemy deals " << enemy_damage_dealt << " each round" << endl;
    
    while (1)
    {
        enemy_hit_points -= player_damage_dealt;
        if (enemy_hit_points <= 0)
        {
            cout << " Player wins" << endl;
            return true;
        }
        
        player_hit_points -= enemy_damage_dealt;
        if (player_hit_points <= 0)
        {
            cout << " Enemy wins" << endl;
            return false;
        }
    }
    cerr << "SHOULD NOT HAVE GOTTEN HERE" << endl;
    return false;
};
*/

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
}

string AocDay22::part1(string filename, vector<string> extra_args)
{
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
