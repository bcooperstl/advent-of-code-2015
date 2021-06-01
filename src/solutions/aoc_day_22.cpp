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
