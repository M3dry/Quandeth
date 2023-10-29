#pragma once

#include <raylib.h>
#include <vector>

#include "ability.hpp"

#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 35

class Player {
public:
    Rectangle rectangle;
    int max_health;
    int health;
    int speed;
    int dash_cooldown;
    int last_dashed;
    std::vector<Ability> abilities;
    Color color;

    Player(Vector2 position, int health, int speed, int dash_cooldown, std::vector<Ability> abilities, Color color = WHITE)
        : rectangle((Rectangle){ position.x, position.y, PLAYER_WIDTH, PLAYER_HEIGHT }), max_health(health), health(health), speed(speed), dash_cooldown(dash_cooldown), last_dashed(-dash_cooldown), abilities(abilities), color(color) {};
    int dash_status(int ticks);
    void draw();
};
