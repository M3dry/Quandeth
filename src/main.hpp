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
    bool map_open;
    Rectangle map_rec;
    Camera2D camera;
    Camera2D map_cam;
    RenderTexture camera_r;
    RenderTexture map_cam_r;

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
