#include <raylib.h>
#include <vector>
#include <stdio.h>

#include "player.hpp"

const float Player::PLAYER_WIDTH = 35;
const float Player::PLAYER_HEIGHT = 35;

int Player::dash_status(int ticks) {
    int status = ticks - last_dashed;
    if (status > dash_cooldown) status = dash_cooldown;
    return status;
}

void Player::draw() {
    DrawRectangleRec(rectangle, color);
}
