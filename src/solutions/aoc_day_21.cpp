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

AocDay21::init_items()
{
    m_weapons[0].name = "No Weapon";
    m_weapons[0].cost = 0;
    m_weapons[0].damage = 0;
    m_weapons[0].armor = 0;
    
    m_weapons[1].name = "Dagger";
    m_weapons[1].cost = 8;
    m_weapons[1].damage = 4;
    m_weapons[1].armor = 0;
    
    m_weapons[2].name = "Shortsword";
    m_weapons[2].cost = 10;
    m_weapons[2].damage = 5;
    m_weapons[2].armor = 0;
    
    m_weapons[3].name = "Warhammer";
    m_weapons[3].cost = 25;
    m_weapons[3].damage = 6;
    m_weapons[3].armor = 0;
    
    m_weapons[4].name = "Longsword";
    m_weapons[4].cost = 40;
    m_weapons[4].damage = 7;
    m_weapons[4].armor = 0;
    
    m_weapons[5].name = "Greataxe";
    m_weapons[5].cost = 74;
    m_weapons[5].damage = 8;
    m_weapons[5].armor = 0;
    
    
    m_armor[0].name = "No Armor";
    m_armor[0].cost = 0;
    m_armor[0].damage = 0;
    m_armor[0].armor = 0;
    
    m_armor[1].name = "Leather";
    m_armor[1].cost = 13;
    m_armor[1].damage = 0;
    m_armor[1].armor = 1;
    
    m_armor[2].name = "Chainmail";
    m_armor[2].cost = 31;
    m_armor[2].damage = 0;
    m_armor[2].armor = 2;
    
    m_armor[3].name = "Splintmail";
    m_armor[3].cost = 53;
    m_armor[3].damage = 0;
    m_armor[3].armor = 3;
    
    m_armor[4].name = "Bandedmail";
    m_armor[4].cost = 75;
    m_armor[4].damage = 0;
    m_armor[4].armor = 4;
    
    m_armor[5].name = "Platemail";
    m_armor[5].cost = 102;
    m_armor[5].damage = 0;
    m_armor[5].armor = 5;
    
    
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
Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3
*/

int AocDay21::parse_input(string filename, Ingredient ** ingredients)
{
    int count = 0;
    for (int i=0; i<MAX_INGREDIENTS; i++)
    {
        ingredients[i] = NULL;
    }
    
    map<string, Ingredient *> lookup;
    
    FileUtils fileutils;
    vector<vector<string>> lines;

    // Going to let the split string parser take care of removing colons and commas.
    char delims[3];
    delims[0]=' ';
    delims[1]=':';
    delims[2]=',';
    if (!fileutils.read_as_list_of_split_strings(filename, lines, delims, 3, '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return 0;
    }

    for (int i=0; i<lines.size(); i++)
    {
        ingredients[i] = new Ingredient();
        ingredients[i]->name = lines[i][0];
        ingredients[i]->capacity   = strtol(lines[i][2].c_str(), NULL, 10);
        ingredients[i]->durability = strtol(lines[i][4].c_str(), NULL, 10);
        ingredients[i]->flavor     = strtol(lines[i][6].c_str(), NULL, 10);
        ingredients[i]->texture    = strtol(lines[i][8].c_str(), NULL, 10);
        ingredients[i]->calories   = strtol(lines[i][10].c_str(), NULL, 10);
        
        cout << "Setting " << ingredients[i]->name << " with capacity " << ingredients[i]->capacity
             << " durability " << ingredients[i]->durability
             << " flavor " << ingredients[i]->flavor
             << " texture " <<ingredients[i]->texture
             << " calories " << ingredients[i]->calories << endl;
        
        count++;
    }
    
    cout << "count is " << count << endl;
    return count;
}

string AocDay21::part1(string filename, vector<string> extra_args)
{
    Ingredient * ingredients[MAX_INGREDIENTS];
    int num_ingredients = parse_input(filename, ingredients);
    
    long high_score = 0;
    
    if (num_ingredients == 2)
    {
        long quantities[2];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            quantities[1] = 100 - quantities[0];
            long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity;
            long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability;
            long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor;
            long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture;
            if (total_capacity < 0)
            {
                total_capacity = 0;
            }
            if (total_durability < 0)
            {
                total_durability = 0;
            }
            if (total_flavor < 0)
            {
                total_flavor = 0;
            }
            if (total_texture < 0)
            {
                total_texture = 0;
            }
            long score = total_capacity * total_durability * total_flavor * total_texture;

            if (score > high_score)
            {
                cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name << " and " << quantities[1] << " of " << ingredients[1]->name << endl;
                high_score = score;
            }
        }
    }
    else if (num_ingredients == 4)
    {
        long quantities[4];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            for (quantities[1] = 0; quantities[1]<=(100 - quantities[0]); quantities[1]++)
            {
                for (quantities[2] = 0; quantities[2]<=(100 - quantities[0] - quantities[1]); quantities[2]++)
                {
                    quantities[3] = 100 - quantities[0] - quantities[1] - quantities[2];
                    long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity   + quantities[2]*ingredients[2]->capacity   + quantities[3]*ingredients[3]->capacity;
                    long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability + quantities[2]*ingredients[2]->durability + quantities[3]*ingredients[3]->durability;
                    long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor     + quantities[2]*ingredients[2]->flavor     + quantities[3]*ingredients[3]->flavor;
                    long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture    + quantities[2]*ingredients[2]->texture    + quantities[3]*ingredients[3]->texture;
                    if (total_capacity < 0)
                    {
                        total_capacity = 0;
                    }
                    if (total_durability < 0)
                    {
                        total_durability = 0;
                    }
                    if (total_flavor < 0)
                    {
                        total_flavor = 0;
                    }
                    if (total_texture < 0)
                    {
                        total_texture = 0;
                    }
                    long score = total_capacity * total_durability * total_flavor * total_texture;
                    if (score > high_score)
                    {
                        cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name 
                        << " and " << quantities[1] << " of " << ingredients[1]->name 
                        << " and " << quantities[2] << " of " << ingredients[2]->name 
                        << " and " << quantities[3] << " of " << ingredients[3]->name << endl;
                        high_score = score;
                    }
                }
            }
        }        
    }
    else
    {
        cerr << "INVALID NUMBER OF INGREDIENTS " << num_ingredients << endl;
    }
    
    for (int i=0; i<num_ingredients; i++)
    {
        delete ingredients[i];
    }
    
    ostringstream out;
    out << high_score;
    return out.str();
}

string AocDay21::part2(string filename, vector<string> extra_args)
{
    Ingredient * ingredients[MAX_INGREDIENTS];
    int num_ingredients = parse_input(filename, ingredients);
    
    long high_score = 0;
    long target_calories = 500;
    
    if (num_ingredients == 2)
    {
        long quantities[2];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            quantities[1] = 100 - quantities[0];
            long total_calories   = quantities[0]*ingredients[0]->calories   + quantities[1]*ingredients[1]->calories;
            if (total_calories == target_calories)
            {
                long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity;
                long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability;
                long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor;
                long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture;
                if (total_capacity < 0)
                {
                    total_capacity = 0;
                }
                if (total_durability < 0)
                {
                    total_durability = 0;
                }
                if (total_flavor < 0)
                {
                    total_flavor = 0;
                }
                if (total_texture < 0)
                {
                    total_texture = 0;
                }
                long score = total_capacity * total_durability * total_flavor * total_texture;
    
                if (score > high_score)
                {
                    cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name << " and " << quantities[1] << " of " << ingredients[1]->name << endl;
                    high_score = score;                
                }
            }
        }
    }
    else if (num_ingredients == 4)
    {
        long quantities[4];
        for (quantities[0] = 0; quantities[0]<=100; quantities[0]++)
        {
            for (quantities[1] = 0; quantities[1]<=(100 - quantities[0]); quantities[1]++)
            {
                for (quantities[2] = 0; quantities[2]<=(100 - quantities[0] - quantities[1]); quantities[2]++)
                {
                    quantities[3] = 100 - quantities[0] - quantities[1] - quantities[2];
                    long total_calories   = quantities[0]*ingredients[0]->calories   + quantities[1]*ingredients[1]->calories   + quantities[2]*ingredients[2]->calories   + quantities[3]*ingredients[3]->calories;
                    if (total_calories == target_calories)
                    {
                        long total_capacity   = quantities[0]*ingredients[0]->capacity   + quantities[1]*ingredients[1]->capacity   + quantities[2]*ingredients[2]->capacity   + quantities[3]*ingredients[3]->capacity;
                        long total_durability = quantities[0]*ingredients[0]->durability + quantities[1]*ingredients[1]->durability + quantities[2]*ingredients[2]->durability + quantities[3]*ingredients[3]->durability;
                        long total_flavor     = quantities[0]*ingredients[0]->flavor     + quantities[1]*ingredients[1]->flavor     + quantities[2]*ingredients[2]->flavor     + quantities[3]*ingredients[3]->flavor;
                        long total_texture    = quantities[0]*ingredients[0]->texture    + quantities[1]*ingredients[1]->texture    + quantities[2]*ingredients[2]->texture    + quantities[3]*ingredients[3]->texture;
                        if (total_capacity < 0)
                        {
                            total_capacity = 0;
                        }
                        if (total_durability < 0)
                        {
                            total_durability = 0;
                        }
                        if (total_flavor < 0)
                        {
                            total_flavor = 0;
                        }
                        if (total_texture < 0)
                        {
                            total_texture = 0;
                        }
                        long score = total_capacity * total_durability * total_flavor * total_texture;
                        if (score > high_score)
                        {
                            cout << "Setting new high score of " << score << " with " << quantities[0] << " of " << ingredients[0]->name 
                            << " and " << quantities[1] << " of " << ingredients[1]->name 
                            << " and " << quantities[2] << " of " << ingredients[2]->name 
                            << " and " << quantities[3] << " of " << ingredients[3]->name << endl;
                            high_score = score;
                        }
                    }
                }
            }
        }        
    }
    else
    {
        cerr << "INVALID NUMBER OF INGREDIENTS " << num_ingredients << endl;
    }
    
    for (int i=0; i<num_ingredients; i++)
    {
        delete ingredients[i];
    }
    
    ostringstream out;
    out << high_score;
    return out.str();
}
