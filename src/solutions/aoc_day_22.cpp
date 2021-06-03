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
        //cout << "Unable to call " << m_spells[spell_number].name 
        //     << " because its cost is " << m_spells[spell_number].cost 
        //     << " and player only has " << turn_stats[current_turn].player_mana << endl;
        return false;
    }
    
    // if the spell does not have a num_turns value, we can turn true here. it can be called any time
    if (m_spells[spell_number].num_turns == 0)
    {
        //cout << "Able to call " << m_spells[spell_number].name << " which does not have a number of turns" << endl;
        return true;
    }
    
    for (int turn = current_turn-1; ((turn >= 0) && (turn >= (current_turn - m_spells[spell_number].num_turns ))); turn--)
    {
        if (turn_stats[turn].last_spell_played == spell_number)
        {
            //cout << "Unable to call " << m_spells[spell_number].name 
            //    << " because its num_turns is " << m_spells[spell_number].num_turns 
            //    << " and it was played on turn " << turn << " with current turn " << current_turn << endl;
            return false;
        }
    }
    
    //cout << "Able to call " << m_spells[spell_number].name 
    //     << " because it was not played in the prior " << m_spells[spell_number].num_turns 
    //     << " with current turn " << current_turn << endl;
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

// clear the memory structure for a turn
void AocDay22::clear_turn(GameStats * turn_stats, int current_turn)
{
    memset(&turn_stats[current_turn], 0, sizeof(struct GameStats));
    turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
}

// done the first time a turn is initialized. Will mark that no spell has been played
void AocDay22::init_turn(GameStats * turn_stats, int current_turn, bool apply_hard_mode_adjustment)
{
    reinit_turn(turn_stats, current_turn, apply_hard_mode_adjustment);
    turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
}

// done to reset to play the next spell. does not reset teh last_spell_played value
void AocDay22::reinit_turn(GameStats * turn_stats, int current_turn, bool apply_hard_mode_adjustment)
{
    int prior_turn = current_turn-1;
    //cout << "Initializing turn " << current_turn << endl;
    turn_stats[current_turn].turn_number = current_turn;
    turn_stats[current_turn].player_hit_points = turn_stats[prior_turn].player_hit_points;
    turn_stats[current_turn].player_armor = turn_stats[prior_turn].player_armor;
    turn_stats[current_turn].player_mana = turn_stats[prior_turn].player_mana;
    turn_stats[current_turn].player_damage = turn_stats[prior_turn].player_damage;
    turn_stats[current_turn].boss_hit_points = turn_stats[prior_turn].boss_hit_points;
    turn_stats[current_turn].boss_damage = turn_stats[prior_turn].boss_damage;
    //turn_stats[current_turn].last_spell_played = SPELL_NONE_OR_BOSS;
    turn_stats[current_turn].player_total_mana_spent = turn_stats[prior_turn].player_total_mana_spent;
    
    // if hard mode adjustment is to be applied, we need to do the action here of decrementing the player's hit points by 1
    // if the causes the player to go to 0, we need to not apply the effects so that the game will end correctly
    
    if (apply_hard_mode_adjustment)
    {
        turn_stats[current_turn].player_hit_points--;
        if (turn_stats[current_turn].player_hit_points <= 0)
        {
            //cout << "Hard mode causes this to end early" << endl;
            return;
        }
    }
    
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
                    //cout << " Armor boost from turn " << target_turn 
                    //     << " has expired. Removing the value of " << m_spells[i].armor_boost 
                    //     << " from spell " << m_spells[i].name << endl;
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
                    //cout << " Dealing start of turn damage of " << m_spells[i].start_turn_damage_dealt
                    //     << " from spell " << m_spells[i].name 
                    //     << " called on turn " << turn << endl;
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
                    //cout << " Mining mana to increase by " << m_spells[i].start_turn_mana_mined
                    //     << " from spell " << m_spells[i].name 
                    //     << " called on turn " << turn << endl;
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
    //cout << "Applying spell " << m_spells[spell_number].name << " on turn " << current_turn << endl;
    turn_stats[current_turn].player_mana -= m_spells[spell_number].cost;
    //cout << " Decrementing player mana by " << m_spells[spell_number].cost << " to go to " << turn_stats[current_turn].player_mana << endl;
    turn_stats[current_turn].player_total_mana_spent += m_spells[spell_number].cost;
    
    if (m_spells[spell_number].armor_boost > 0)
    {
        turn_stats[current_turn].player_armor += m_spells[spell_number].armor_boost;
        //cout << " Applying " << m_spells[spell_number].armor_boost 
        //     << " armor boost to the player, resulting in armor incrementing to " <<  turn_stats[current_turn].player_armor << endl;
    }
    
    if (m_spells[spell_number].instant_damage_dealt > 0)
    {
        turn_stats[current_turn].boss_hit_points -= m_spells[spell_number].instant_damage_dealt;
        //cout << " Dealing " << m_spells[spell_number].instant_damage_dealt 
        //     << " damage to boss resulting in their hit points at " << turn_stats[current_turn].boss_hit_points << endl;
    }
    
    if (m_spells[spell_number].instant_healing_applied > 0)
    {
        turn_stats[current_turn].player_hit_points += m_spells[spell_number].instant_healing_applied;
        //cout << " Healing " << m_spells[spell_number].instant_healing_applied 
        //     << " hit points to bring player to " << turn_stats[current_turn].player_hit_points << endl;
    }
    
    turn_stats[current_turn].last_spell_played = spell_number;
    
    return;
}

void AocDay22::boss_attack(GameStats * turn_stats, int current_turn)
{
    //cout << "Boss attacking on turn " << current_turn << endl;
    
    int player_hit_points = turn_stats[current_turn].player_hit_points;
    int boss_hit_points = turn_stats[current_turn].boss_hit_points;
    
    //cout << " Player starts with " << player_hit_points << " hit points" << endl;
    //cout << " Boss starts with " << boss_hit_points << " hit points" << endl;
    
    int boss_damage_dealt = turn_stats[current_turn].boss_damage - turn_stats[current_turn].player_armor;
    
    if (boss_damage_dealt < 1)
    {
        boss_damage_dealt = 1;
    }
    
    //cout << " The boss does " << boss_damage_dealt 
    //     << " damage based on having " << turn_stats[current_turn].boss_damage
    //     << " damage against " << turn_stats[current_turn].player_armor
    //     << " player armor" << endl;
    
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
            cerr << "INVALID GAME SCENARIO - BOTH LOSS" << endl;
            return GAME_NOT_OVER;
        }
    }
}

int AocDay22::find_player_win_least_mana(GameStats * turn_stats, bool is_hard_mode)
{
    int least_mana_used = INT_MAX;
    int current_turn = 1;
    clear_turn(turn_stats, 1); // clear the stats for turn 1
    while (current_turn > 0)
    {
        // for every turn, we first run the init routine
        if (current_turn % 2 == 1) // player turn - turns 1, 3, 5, etc
        {
            bool is_first = false;
            int next_spell_to_try = 0;
            // first step - initialize the turn. will run any effects
            //cout << "Turn " << current_turn << " is a turn for the player" << endl;
            
            if (turn_stats[current_turn].last_spell_played == SPELL_NONE_OR_BOSS)
            {
                //cout << " No spell tried at this level yet. Doing full init" << endl;
                init_turn(turn_stats, current_turn, is_hard_mode);
                is_first = true;
            }
            else if (turn_stats[current_turn].last_spell_played >= (MAX_SPELLS-1))
            {
                //cout << " All spells have been tried at this level yet. Backing up 2 to the prior player turn" << endl;
                current_turn -= 2;
                continue;
            }
            else
            {
                reinit_turn(turn_stats, current_turn, is_hard_mode);
                //cout << " A spell has been tried at this level already. Doing reinit" << endl;
                next_spell_to_try = turn_stats[current_turn].last_spell_played + 1;
            }
            
            int game_status = is_game_over(turn_stats, current_turn);
            if (game_status == GAME_OVER_PLAYER_WON) // the player can win based on the init_turn effects
            {
                //cout << " The Player won based on the init_turn rules" << endl;
                //cout << " The player has used " << turn_stats[current_turn].player_total_mana_spent << " mana" << endl;
                if (turn_stats[current_turn].player_total_mana_spent < least_mana_used)
                {
                    least_mana_used = turn_stats[current_turn].player_total_mana_spent;
                    cout << " *** NEW LEAST MANA USED VALUE OF " << least_mana_used << endl;
                }
                current_turn -= 2; // go back to the prior turn to try a different spell to see if it gets a better result;
                continue;
            }
            else if (game_status == GAME_OVER_BOSS_WON) // the player can win based on the init_turn effects when hard mode is added
            {
                //cout << " The Boss won based on the init_turn rules" << endl;
                current_turn -= 2; // go back to the prior turn to try a different spell to see if it gets a better result;
                continue;
            }
            
            // determine next spell to cast
            while (next_spell_to_try < MAX_SPELLS)
            {
                if (can_cast_spell(turn_stats, current_turn, next_spell_to_try))
                {
                    //cout << " Can cast " << m_spells[next_spell_to_try].name << " next" << endl;
                    break;
                }
                else
                {
                    //cout << " Cannot cast " << m_spells[next_spell_to_try].name << endl;
                }
                next_spell_to_try++;
            }
            
            // there may be no more to cast here. go back up to the prior player turn if so
            if (next_spell_to_try == MAX_SPELLS)
            {
                // there are no spells to cast
                if (is_first)
                {
                    //cout << " There are no spells that the player can cast at this level. The player loses" << endl;
                }
                else
                {
                    //cout << " There are no further spells the player can cast at this level. Going back 2 levels to try other permutatiosn" << endl;
                }
                current_turn -= 2;
                continue;
            }
            
            // ok we can finally cast a spell here
            //cout << " Using spell " << m_spells[next_spell_to_try].name << endl;
            apply_spell(turn_stats, current_turn, next_spell_to_try);
            
            // if we have spent more mana than the best, it doesn't matter if we win, since this won't be the best answer
            if (turn_stats[current_turn].player_total_mana_spent >= least_mana_used)
            {
                //cout << " After casting this spell, the player has used " 
                //     << turn_stats[current_turn].player_total_mana_spent << " mana, which is more than the best value of " << least_mana_used << endl;
                //cout << " No point in continuing down this path. " << endl;
                // not adjusting the current_turn level; the next time through the loop will check the next spell at this level
                continue;
            }
            
            game_status = is_game_over(turn_stats, current_turn);
            if (game_status == GAME_OVER_PLAYER_WON) // only the player can win based on the init_turn effects
            {
                //cout << " The Player won after casting the " <<  m_spells[next_spell_to_try].name << " spell" << endl;
                //cout << " The player has used " << turn_stats[current_turn].player_total_mana_spent << " mana" << endl;
                if (turn_stats[current_turn].player_total_mana_spent < least_mana_used)
                {
                    least_mana_used = turn_stats[current_turn].player_total_mana_spent;
                    cout << " *** NEW LEAST MANA USED VALUE OF " << least_mana_used << endl;
                }
                // not adjusting the current_turn level; the next time through the loop will check the next spell at this level
            }
            else
            {
                current_turn++;
                clear_turn(turn_stats, current_turn);
                //cout << " The game continues with the bosses turn at level " << current_turn << endl;
            }
        }
        else // boss turn - turns 2, 4, 6, etc
        {
            // first step - initialize the turn. will run any effects
            //cout << "Turn " << current_turn << " is a turn for the boss" << endl;
            init_turn(turn_stats, current_turn, false); // never apply the hard mode adjustment on the boss's turns
            int game_status = is_game_over(turn_stats, current_turn);
            if (game_status == GAME_OVER_PLAYER_WON) // only the player can win based on the init_turn effects
            {
                //cout << " The Player won based on the init_turn rules" << endl;
                //cout << " The player has used " << turn_stats[current_turn].player_total_mana_spent << " mana" << endl;
                if (turn_stats[current_turn].player_total_mana_spent < least_mana_used)
                {
                    least_mana_used = turn_stats[current_turn].player_total_mana_spent;
                    cout << " *** NEW LEAST MANA USED VALUE OF " << least_mana_used << endl;
                }
                current_turn--; // go back to the prior turn to try a different spell to see if it gets a better result;
                continue;
            }
            
            // at this point, both players still alive and it is time for the boss to attack
            boss_attack(turn_stats, current_turn);
            game_status = is_game_over(turn_stats, current_turn); // only the boss could achieve a win after the boss_attack phase
            if (game_status == GAME_OVER_BOSS_WON)
            {
                //cout << " The Boss won after the boss_attack phase" << endl;
                current_turn--;
                continue; // go back to the prior turn to see if the player can win based on a different spell
            }
            
            // at this point, we will continue to the next round for the player. Increment the current_turn and reset the data structure for that turn
            current_turn++;
            clear_turn(turn_stats, current_turn);
        }
    }
    return least_mana_used;
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
    
    parse_input(filename, enemy_hit_points, enemy_damage);
    
    setup_turn_0(turn_stats, PLAYER_START_HIT_POINTS, PLAYER_START_ARMOR, PLAYER_START_MANA, PLAYER_START_DAMAGE, enemy_hit_points, enemy_damage);
    
    int least_mana_used = find_player_win_least_mana(turn_stats, false); // not hard mode
    
    ostringstream out;
    out << least_mana_used;
    return out.str();
}

string AocDay22::part2(string filename, vector<string> extra_args)
{
    if (extra_args.size() == 1)
    {
        return run_test_scenario_part1(strtol(extra_args[0].c_str(), NULL, 10));
    }

    GameStats turn_stats[MAX_TURNS + 1]; // add the +1 for the initial setup in turn 0;
    memset(turn_stats, 0, sizeof(struct GameStats) * (MAX_TURNS + 1));
    
    int enemy_hit_points, enemy_damage;
    
    parse_input(filename, enemy_hit_points, enemy_damage);
    
    setup_turn_0(turn_stats, PLAYER_START_HIT_POINTS, PLAYER_START_ARMOR, PLAYER_START_MANA, PLAYER_START_DAMAGE, enemy_hit_points, enemy_damage);
    
    int least_mana_used = find_player_win_least_mana(turn_stats, true); // hard mode
    
    ostringstream out;
    out << least_mana_used;
    return out.str();
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
        
        init_turn(turn_stats, 1, false);
        
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
        
        init_turn(turn_stats, 2, false);
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
        
        init_turn(turn_stats, 3, false);
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

        init_turn(turn_stats, 4, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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
        init_turn(turn_stats, current_turn, false);
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

