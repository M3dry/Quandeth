#include <raylib.h>
#include <raymath.h>
#include <tuple>

#include "main.hpp"
#include "player.hpp"
#include "graph.hpp"

static Vector2 center(Rectangle rec) {
    return (Vector2){ rec.x + rec.width/2, rec.y + rec.height/2 };
}

void Game::process() {
    static const int keys[] = { KEY_W, KEY_A, KEY_S, KEY_D };
    static const Vector2 movements[] = { { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } };
    Vector2 movement = { 0, 0 };

    for (int k = 0; k < 4; k++) {
        if (IsKeyDown(keys[k])) movement = Vector2Add(movement, movements[k]);
    }
    float length = Vector2Length(movement);
    if (length != 1 && length != 0) movement = Vector2Divide(movement, { length, length });

    if (movement.x != 0 || movement.y != 0) {
        player.rectangle.x += player.speed*movement.x;
        if (dungeon.check_collisions(player.rectangle)) {
                player.rectangle.x -= player.speed*movement.x;
        }

        player.rectangle.y += player.speed*movement.y;
        if (dungeon.check_collisions(player.rectangle)) {
                player.rectangle.y -= player.speed*movement.y;
        }

        camera.target = center(player.rectangle);
    }
}

void Game::draw() {
    BeginDrawing();
        ClearBackground(GetColor(0x0f111bff));
        DrawRectangleRec(viewport, BLACK);

        draw_hud();
        BeginMode2D(camera);
            BeginScissorMode(viewport.x, viewport.y, viewport.width, viewport.height);
                dungeon.draw();
                player.draw();
            EndScissorMode();
        EndMode2D();
    EndDrawing();
}

void Game::draw_hud() {
    Rectangle hud_area = (Rectangle){ 0, viewport.height, viewport.width, screen.y - viewport.height };

    DrawRectangle(hud_area.x, hud_area.y, hud_area.width, 5, WHITE);
}

Game::Game(Dungeon dungeon) :
        screen({ (float)GetScreenWidth(), (float)GetScreenHeight() }),
        viewport({ 0, 0, screen.x, screen.y * 0.9f }),
        dungeon(dungeon),
        player(dungeon.spawn, 500, 2, 200, {}) {
    camera = { 0 };
    camera.target = center(player.rectangle);
    camera.offset = (Vector2){ screen.x/2, screen.y/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

Game::~Game() {
    
}

int main(int argc, char** argv) {
    InitWindow(0, 0, "Quandeth");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetExitKey(KEY_NULL);

    auto room0 = graph::Node<Room>(Room(Room::TRoom, { -100, -700, 500, 500 }), { 1 });
    auto hallway1 = graph::Node<Room>(Room(Room::THallway, { 100, -200, 100, 100 }), { 0, 2 });
    auto room1 = graph::Node<Room>(Room(Room::TRoom, { -100, -100, 500, 500 }), { 1, 3 });
    auto hallway2 = graph::Node<Room>(Room(Room::THallway, { 400, 100, 200, 100 }), { 2, 4 });
    auto room2 = graph::Node<Room>(Room(Room::TRoom, { 600, -100, 500, 500 }), { 3 });

    Dungeon dungeon = Dungeon({ 300, 300 }, { room0, hallway1, room1, hallway2, room2 }, 2, {}, GetColor((unsigned int)0x72a4fffff), GetColor(0xff5370ff), GetColor(0xc3e88dff));
    // auto room1 = graph::Node<Room>(Room(Room::TRoom, { -100, -100, 500, 500 }), { 1 });
    // auto hallway2 = graph::Node<Room>(Room(Room::THallway, { 400, 100, 200, 100 }), { 0, 2 });
    // auto room2 = graph::Node<Room>(Room(Room::TRoom, { 600, -100, 500, 500 }), { 1 });

    // Dungeon dungeon = Dungeon({ 300, 300 }, { room1, hallway2, room2 }, 0, {}, GetColor((unsigned int)0x72a4fffff), GetColor(0xff5370ff), GetColor(0xc3e88dff));
    Game game = Game(dungeon);

    bool tick = true;
    double mili_accum = 0;

    while (!WindowShouldClose()) {
        double time = GetTime();
        if (tick) {
            game.process();
            tick = false;
        }

        game.draw();

        mili_accum += GetTime() - time;
        if (mili_accum > 0.005) {
            mili_accum = 0;
            tick = true;
            game.incrTicks();
        }
    }

    CloseWindow();
}
