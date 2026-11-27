#include "gameplay/player.h"
#include "data/content_database.h"
#include "content/ships.h"
#include <cmath>
#include <string>

//Constructor for player class
Player::Player(const ShipDef& ship, int max_lives)
    : x_pos(192),
      y_pos(224),
      id(ship.id),
      width(ship.width),
      height(ship.height),
      max_speed(ship.speed),
      current_speed(max_speed),
      starting_bombs(ship.starting_bombs),
      max_lives(max_lives),
      current_lives(max_lives),
      bomb_type(ship.bomb_type),
      starting_weapon(ship.starting_weapon),
      slow_mode(false),
      continue_used(false),
      diagonal_movement(false)  
{
}
//Function to move player based on input received
void Player::move(double dx, double dy, double dt, bool diagonal) {
    x_pos += dx * current_speed * dt;
    y_pos += dy * current_speed * dt;
    //Debug print
   // fprintf(stderr, "%f %f", x_pos,y_pos);
}
//Functions get x and y coordinates of the player
double Player::x() const {
    return x_pos;
}

double Player::y() const {
    return y_pos;
}
//Function to calculate current player speed as a function of max speed and all speed modifers
void Player::compute_current_speed(Player player, bool diagonal){
    double diagonal_modifier, slow_modifier;
    if(diagonal) diagonal_modifier = sqrt(2);
    else diagonal_modifier = 1;
    if(player.slow_mode) slow_modifier = 0.5;
    else slow_modifier = 1;
    player.current_speed = player.max_speed * diagonal_modifier * slow_modifier;
}
//Width and height getters
int Player::get_width() const {
    return width;
}

int Player::get_height() const {
    return height;
}