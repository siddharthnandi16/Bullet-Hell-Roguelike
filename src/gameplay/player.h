#ifndef SRC_GAMEPLAY_PLAYER_H_
#define SRC_GAMEPLAY_PLAYER_H_

#include <string>
#include <vector>

#include "content/ships.h"
#include "data/content_database.h"
class Player{
    public:
explicit Player(const ShipDef& ship, int max_lives);
void move(double dx, double dy, double dt, bool diagonal);
double x() const;
double y() const;
int get_width() const;
int get_height() const;
int get_current_lives() const;
void compute_current_speed(Player player, bool diagonal);
    private:
double x_pos;
double y_pos;
double max_speed;
double current_speed;
double rank; //Function of score, lives_lost_total and continue_used. Gates access to certain stages
int score;
int current_lives;
int max_lives;
int width;
int height;
int lives_lost_total; //Measures lives lost across a whole run
int lives_lost_stage; //Measures lives lost in the current stage
int lives_lost_wave; //Measures lives lost in the current wave
int starting_bombs;
int current_bombs;
bool slow_mode; //Halves movement speed when active, activated by holding down shift
bool diagonal_movement;
bool continue_used;
std::string id;
std::string bomb_type;
std::string starting_weapon;
std::string special_rules;
};
#endif  // SRC_GAMEPLAY_PLAYER_H_
