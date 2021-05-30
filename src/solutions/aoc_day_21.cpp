#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cstring>

#include "aoc_day_21.h"
#include "file_utils.h"

using namespace std;
using namespace Day21;

namespace Day21
{
    Character::Character(string name, int start_hit_points)
    {
        m_name = name;
        m_start_hit_points = start_hit_points;
    }
    
    int Character::get_start_hit_points()
    {
        return m_start_hit_points;
    }
    
    string Character::get_name()
    {
        return m_name;
    }
    
    Enemy::Enemy(int start_hit_points, int damage, int armor):Character("ENEMY", start_hit_points)
    {
        m_damage = damage;
        m_armor = armor;
    }
    
    int Enemy::get_damage()
    {
        return m_damage;
    }
    
    int Enemy::get_armor()
    {
        return m_armor;
    }
    
    Player::Player(int start_hit_points):Character("PLAYER", start_hit_points)
    {
        m_weapon = NULL;
        m_armor = NULL;
        m_rings[0] = NULL;
        m_rings[1] = NULL;
    }
    
    void Player::set_weapon(Item * weapon)
    {
        m_weapon = weapon;
    }
    
    void Player::set_armor(Item * armor)
    {
        m_armor = armor;
    }
    
    void Player::set_rings(Item * ring1, Item * ring2)
    {
        m_rings[0] = ring1;
        m_rings[1] = ring2;
    }
    
    void Player::display_equipment()
    {
        cout << m_weapon->name << " " << m_armor->name << " " << m_rings[0]->name << " " << m_rings[1]->name;
    }
    
    int Player::get_damage()
    {
        return m_weapon->damage + m_armor->damage + m_rings[0]->damage + m_rings[1]->damage;
    }
    
    int Player::get_armor()
    {
        return m_weapon->armor + m_armor->armor + m_rings[0]->armor + m_rings[1]->armor;
    }
}

AocDay21::AocDay21():AocDay(21)
{
    init_items();
}

AocDay21::~AocDay21()
{
}

void AocDay21::init_items()
{
    //m_weapons[0].name = "No Weapon";
    //m_weapons[0].cost = 0;
    //m_weapons[0].damage = 0;
    //m_weapons[0].armor = 0;
    
    m_weapons[0].name = "Dagger";
    m_weapons[0].cost = 8;
    m_weapons[0].damage = 4;
    m_weapons[0].armor = 0;
    
    m_weapons[1].name = "Shortsword";
    m_weapons[1].cost = 10;
    m_weapons[1].damage = 5;
    m_weapons[1].armor = 0;
    
    m_weapons[2].name = "Warhammer";
    m_weapons[2].cost = 25;
    m_weapons[2].damage = 6;
    m_weapons[2].armor = 0;
    
    m_weapons[3].name = "Longsword";
    m_weapons[3].cost = 40;
    m_weapons[3].damage = 7;
    m_weapons[3].armor = 0;
    
    m_weapons[4].name = "Greataxe";
    m_weapons[4].cost = 74;
    m_weapons[4].damage = 8;
    m_weapons[4].armor = 0;
    
    
    m_armors[0].name = "No Armor";
    m_armors[0].cost = 0;
    m_armors[0].damage = 0;
    m_armors[0].armor = 0;
    
    m_armors[1].name = "Leather";
    m_armors[1].cost = 13;
    m_armors[1].damage = 0;
    m_armors[1].armor = 1;
    
    m_armors[2].name = "Chainmail";
    m_armors[2].cost = 31;
    m_armors[2].damage = 0;
    m_armors[2].armor = 2;
    
    m_armors[3].name = "Splintmail";
    m_armors[3].cost = 53;
    m_armors[3].damage = 0;
    m_armors[3].armor = 3;
    
    m_armors[4].name = "Bandedmail";
    m_armors[4].cost = 75;
    m_armors[4].damage = 0;
    m_armors[4].armor = 4;
    
    m_armors[5].name = "Platemail";
    m_armors[5].cost = 102;
    m_armors[5].damage = 0;
    m_armors[5].armor = 5;
    
    
    m_rings[0].name = "No Ring";
    m_rings[0].cost = 0;
    m_rings[0].damage = 0;
    m_rings[0].armor = 0;
    
    m_rings[1].name = "Damage +1";
    m_rings[1].cost = 25;
    m_rings[1].damage = 1;
    m_rings[1].armor = 0;
    
    m_rings[2].name = "Damage +2";
    m_rings[2].cost = 50;
    m_rings[2].damage = 2;
    m_rings[2].armor = 0;
    
    m_rings[3].name = "Damage +3";
    m_rings[3].cost = 100;
    m_rings[3].damage = 3;
    m_rings[3].armor = 0;
    
    m_rings[4].name = "Defense +1";
    m_rings[4].cost = 20;
    m_rings[4].damage = 0;
    m_rings[4].armor = 1;
    
    m_rings[5].name = "Defense +2";
    m_rings[5].cost = 40;
    m_rings[5].damage = 0;
    m_rings[5].armor = 2;
    
    m_rings[6].name = "Defense +3";
    m_rings[6].cost = 80;
    m_rings[6].damage = 0;
    m_rings[6].armor = 3;
}

/* Input format:
Hit Points: 109
Damage: 8
Armor: 2
*/

void AocDay21::parse_input(string filename, int & enemy_hit_points, int & enemy_damage, int & enemy_armor)
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
    enemy_armor = strtol(lines[2][1].c_str(), NULL, 10);
    
    return;
}

int AocDay21::get_cost(Item * weapon, Item * armor, Item * ring1, Item * ring2)
{
    return weapon->cost + armor->cost + ring1->cost + ring2->cost;
}

// Returns true if the player wins or false if the enemy wins
bool AocDay21::battle(Player * player, Enemy * enemy)
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

string AocDay21::part1(string filename, vector<string> extra_args)
{
    Player player(100); // start player with 100 hit points
    
    int enemy_hit_points, enemy_damage, enemy_armor;
    
    int best_cost = INT_MAX;
    
    parse_input(filename, enemy_hit_points, enemy_damage, enemy_armor);
    
    Enemy enemy(enemy_hit_points, enemy_damage, enemy_armor);
    
    for (int weapon_idx=0; weapon_idx < NUM_WEAPONS; weapon_idx++)
    {
        Item * weapon = &m_weapons[weapon_idx];
        player.set_weapon(weapon);
        for (int armor_idx=0; armor_idx <= NUM_ARMOR; armor_idx++)
        {
            Item * armor = &m_armors[armor_idx];
            player.set_armor(armor);

            // working through the permutations
            for (int ring1_idx=0; ring1_idx < NUM_RINGS; ring1_idx++) // do not use the last ring on the first hand
            {
                for (int ring2_idx=ring1_idx; ring2_idx <= NUM_RINGS; ring2_idx++) // include the last ring for the second hand
                {
                    if (ring1_idx == ring2_idx && ring1_idx != 0)
                    {
                        continue; // only allow duplicate item for no rings
                    }
                    
                    Item * ring1 = &m_rings[ring1_idx];
                    Item * ring2 = &m_rings[ring2_idx];

                    player.set_rings(ring1, ring2);
                    
                    int cost = get_cost(weapon, armor, ring1, ring2);
                    if (cost < best_cost)
                    {
                        if (battle(&player, &enemy))
                        {
                            cout << "NEW BEST COST OF " << cost << endl;
                            best_cost = cost;
                        }
                    }
                    else
                    {
                        cout << "Skipping for player with ";
                        player.display_equipment();
                        cout << " because the cost of " << cost << " is worse than best cost " << best_cost << endl;
                    }
                }
            }
        }
    }
    
    ostringstream out;
    out << best_cost;
    return out.str();
}

string AocDay21::part2(string filename, vector<string> extra_args)
{
    Player player(100); // start player with 100 hit points
    
    int enemy_hit_points, enemy_damage, enemy_armor;
    
    int best_cost = -1;
    
    parse_input(filename, enemy_hit_points, enemy_damage, enemy_armor);
    
    Enemy enemy(enemy_hit_points, enemy_damage, enemy_armor);
    
    for (int weapon_idx=0; weapon_idx < NUM_WEAPONS; weapon_idx++)
    {
        Item * weapon = &m_weapons[weapon_idx];
        player.set_weapon(weapon);
        for (int armor_idx=0; armor_idx <= NUM_ARMOR; armor_idx++)
        {
            Item * armor = &m_armors[armor_idx];
            player.set_armor(armor);

            // working through the permutations
            for (int ring1_idx=0; ring1_idx < NUM_RINGS; ring1_idx++) // do not use the last ring on the first hand
            {
                for (int ring2_idx=ring1_idx; ring2_idx <= NUM_RINGS; ring2_idx++) // include the last ring for the second hand
                {
                    if ((ring1_idx == ring2_idx) && (ring1_idx != 0))
                    {
                        continue; // only allow duplicate item for no rings
                    }
                    
                    Item * ring1 = &m_rings[ring1_idx];
                    Item * ring2 = &m_rings[ring2_idx];

                    player.set_rings(ring1, ring2);
                    
                    int cost = get_cost(weapon, armor, ring1, ring2);
                    if (cost > best_cost)
                    {
                        if (!battle(&player, &enemy)) // looking to lose the fight
                        {
                            cout << "NEW BEST COST OF " << cost << " TO LOSE THE FIGHT" << endl;
                            best_cost = cost;
                        }
                    }
                    else
                    {
                        cout << "Skipping for player with ";
                        player.display_equipment();
                        cout << " because the cost of " << cost << " is less than most spent " << best_cost << endl;
                    }
                }
            }
        }
    }
    
    ostringstream out;
    out << best_cost;
    return out.str();
}
