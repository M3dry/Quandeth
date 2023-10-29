#pragma once

#include "player.hpp"
#include "dungeon.hpp"

class Game {
private:
    int ticks;

    void draw_hud();

public:
    Vector2 screen;
    Rectangle viewport;
    Dungeon dungeon;
    Player player;
    Camera2D camera;

    Game(Dungeon dungeon);
    void process();
    void draw();
    ~Game();

    int getTicks() {
        return ticks;
    }

    void incrTicks() {
        ticks++;
    }
};
