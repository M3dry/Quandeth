#pragma once

#include <raylib.h>
#include <vector>

#include "ability.hpp"

class Player {
public:
    static const float PLAYER_WIDTH;
    static const float PLAYER_HEIGHT;

    Rectangle rectangle;
    int max_health;
    int health;
    int speed;
    int dash_cooldown;
    int last_dashed;
    std::vector<Ability> abilities;
    std::size_t room;
    Color color;

    Player(Vector2 position, int health, int speed, int dash_cooldown, std::vector<Ability> abilities, std::size_t room, Color color = WHITE)
        : rectangle((Rectangle){ position.x, position.y, Player::PLAYER_WIDTH, Player::PLAYER_HEIGHT }), max_health(health), health(health), speed(speed), dash_cooldown(dash_cooldown), last_dashed(-dash_cooldown), abilities(abilities), room(room), color(color) {};
    int dash_status(int ticks);
    void draw();
};
