#include <raylib.h>
#include <vector>

#include "player.hpp"

int Player::dash_status(int ticks) {
    int status = ticks - last_dashed;
    if (status > dash_cooldown) status = dash_cooldown;
    return status;
}

void Player::draw() {
    DrawRectangleRec(rectangle, color);
}
