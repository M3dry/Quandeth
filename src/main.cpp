#include <raylib.h>
#include <raymath.h>
#include <tuple>

#include "main.hpp"
#include "player.hpp"
#include "graph.hpp"
#include "util.hpp"

#include "quant.hpp"

void Game::process() {
    PollInputEvents();

    map_open = IsKeyDown(KEY_TAB);
    map_cam.zoom += GetMouseWheelMove()*0.05f;
    if (map_cam.zoom < 0.1) map_cam.zoom = 0.1;

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
        if (dungeon.check_collisions(player.rectangle, player.room)) {
                player.rectangle.x -= player.speed*movement.x;
        }

        player.rectangle.y += player.speed*movement.y;
        if (dungeon.check_collisions(player.rectangle, player.room)) {
                player.rectangle.y -= player.speed*movement.y;
        }

        camera.target = util::center(player.rectangle);
        if (map_open) map_cam.target = util::center(util::scale_rec(player.rectangle, 0.2));
    }
}

void Game::draw() {
    // Dungeon area
    BeginTextureMode(camera_r);
        ClearBackground(BLACK);
        BeginMode2D(camera);
            dungeon.draw();
            player.draw();
        EndMode2D();
    EndTextureMode();

    if (map_open) {
        BeginTextureMode(map_cam_r);
            ClearBackground(BLACK);
            BeginMode2D(map_cam);
                dungeon.draw(true, 0.2);
                DrawCircleV(util::center(util::scale_rec(player.rectangle, 0.2)), 3, WHITE);
            EndMode2D();
        EndTextureMode();
    }

    BeginDrawing();
        ClearBackground(GetColor(0x0f111bff));

        DrawTexturePro(camera_r.texture, { 0, 0, viewport.width, -viewport.height }, { 0, 0, viewport.width, viewport.height }, { 0, 0, }, 0.0f, WHITE);
        if (map_open) {
            DrawRectangle(map_rec.x - 5, map_rec.y - 5, map_rec.width + 10, map_rec.height + 10, WHITE);
            DrawTexturePro(map_cam_r.texture,
                           { map_rec.x, map_rec.y, map_rec.width, -map_rec.height },
                           { map_rec.x, map_rec.y, map_rec.width, map_rec.height },
                           { 0, 0 },
                           0.0f, WHITE);
        }

        draw_hud();
    EndDrawing();

    SwapScreenBuffer();
}

void Game::draw_hud() {
    Rectangle hud_area = (Rectangle){ 0, viewport.height, viewport.width, screen.y - viewport.height };

    DrawRectangle(hud_area.x, hud_area.y, hud_area.width, 5, WHITE);
}

Game::Game(Dungeon dungeon) :
        screen({ (float)GetScreenWidth(), (float)GetScreenHeight() }),
        viewport({ 0, 0, screen.x, screen.y * 0.9f }),
        dungeon(dungeon),
        player(dungeon.spawn, 500, 2, 200, {}, dungeon.rooms.root),
        map_open(false),
        map_rec({ viewport.x + viewport.width*0.2/2, viewport.y + viewport.height*0.2/2, viewport.width*0.8, viewport.height*0.8 }) {
    camera = { 0 };
    camera.target = util::center(player.rectangle);
    camera.offset = { screen.x/2, screen.y/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    map_cam = camera;
    map_cam.target = util::center(util::scale_rec(player.rectangle, 0.2));
    map_cam.offset = { (map_rec.x + map_rec.width)/2, (map_rec.y + map_rec.height)/2 }; // idk why I have to add the x and y values

    camera_r = LoadRenderTexture(viewport.width, viewport.height);
    map_cam_r = LoadRenderTexture(map_rec.x + map_rec.width, map_rec.y + map_rec.height); // also dk why I have to add the x and y values here
}

Game::~Game() {
    UnloadRenderTexture(camera_r);
    UnloadRenderTexture(map_cam_r);
}

int main(int argc, char** argv) {
    quant_test();
    // InitWindow(0, 0, "Quandeth");
    // SetWindowState(FLAG_FULLSCREEN_MODE);
    // SetExitKey(KEY_NULL);

    // auto scale = 2.5;
    // auto spawn = graph::Node<Room>(Room(Room::TRoom, util::scale_rec({ 0, 0, 500, 500 }, scale)), { 1, 2, 7, 8 });
    // auto exit1 = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ 100, -100, 100, 100 }, scale)), { 0, 3 });
    // auto exit2 = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ 300, -100, 100, 100 }, scale)), { 0, 4 });
    // auto exit1hallway = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ -300, -200, 500, 100 }, scale)), { 1, 5 });
    // auto exit2hallway = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ 300, -200, 500, 100 }, scale)), { 2, 6 });
    // auto room1 = graph::Node<Room>(Room(Room::TRoom, util::scale_rec({ -800, -400, 500, 500 }, scale)), { 3, 9 });
    // auto room2 = graph::Node<Room>(Room(Room::TRoom, util::scale_rec({ 800, -400, 500, 500 }, scale)), { 4, 10 });
    // auto exit3 = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ -600, 200, 600, 100 }, scale)), { 0, 9 });
    // auto exit4 = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ 500, 200, 600, 100 }, scale)), { 0, 10 });
    // auto exit3hallway = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ -600, 100, 100, 100 }, scale)), { 7, 5 });
    // auto exit4hallway = graph::Node<Room>(Room(Room::THallway, util::scale_rec({ 1000, 100, 100, 100 }, scale)), { 8, 6 });

    // Dungeon dungeon = Dungeon(util::center(spawn.value.rectangle, { Player::PLAYER_WIDTH, Player::PLAYER_HEIGHT }), { spawn, exit1, exit2, exit1hallway, exit2hallway, room1, room2, exit3, exit4, exit3hallway, exit4hallway }, 0);
    // Game game = Game(dungeon);

    // bool tick = true;
    // double mili_accum = 0;

    // while (!WindowShouldClose()) {
    //     double time = GetTime();
    //     if (tick) {
    //         game.process();
    //         tick = false;
    //     }

    //     game.draw();

    //     mili_accum += GetTime() - time;
    //     if (mili_accum > 0.005) {
    //         mili_accum = 0;
    //         tick = true;
    //         game.incrTicks();
    //     }
    // }

    // CloseWindow();
}
