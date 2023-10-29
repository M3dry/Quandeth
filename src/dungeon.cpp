#include <raylib.h>
#include <vector>
#include <tuple>
#include <stdio.h>

#include "dungeon.hpp"
#include "graph.hpp"

Dungeon::Dungeon(Vector2 spawn, vector<graph::Node<Room>> rooms, std::size_t spawn_room, vector<const char *> texture_paths = {},
                 Color room_floor = GetColor(0x292d3eff), Color hallway_floor = GetColor(0x292d3eff), Color border = WHITE) :
            spawn((Vector2){ rooms[spawn_room].value.rectangle.x + spawn.x, rooms[spawn_room].value.rectangle.y + spawn.y }),
            current_room(spawn_room), rooms(rooms, spawn_room),
            room_floor(room_floor), hallway_floor(hallway_floor), border(border) {
    textures.reserve(texture_paths.size());
    for (std::size_t i = 0; i < texture_paths.size(); i++) {
        textures[i] = LoadTexture(texture_paths[i]);
    }
};

void Dungeon::draw() {
    rooms.walk([&](auto node, const auto& nodes) -> int {
        switch (nodes[node].value.type) {
        case Room::TRoom:
            DrawRectangleRec(nodes[node].value.rectangle, room_floor);
            break;
        case Room::THallway:
            DrawRectangleRec(nodes[node].value.rectangle, hallway_floor);
            break;
        }

        for (const auto& [v, i] : nodes[node].value.obstacles) {
            DrawTextureV(textures[i], v, WHITE);
        }

        return 0;
    });
}

static bool CheckCollisionPointPoly(Vector2 point, Vector2 *points, int pointCount) {
    bool collision = false;

    if (pointCount > 2)
    {
        for (int i = 0; i < pointCount - 1; i++)
        {
            Vector2 vc = points[i];
            Vector2 vn = points[i + 1];

            if ((((vc.y >= point.y) && (vn.y < point.y)) || ((vc.y < point.y) && (vn.y >= point.y))) &&
                 (point.x < ((vn.x - vc.x)*(point.y - vc.y)/(vn.y - vc.y) + vc.x))) collision = !collision;
        }
    }

    return collision;
}

static vector<Vector2> get_verts(const Rectangle& rec) {
    vector<Vector2> vec = {
        { rec.x, rec.y },
        { rec.x + rec.width, rec.y },
        { rec.x + rec.width, rec.y + rec.height },
        { rec.x, rec.y + rec.height }
    };
    return vec;
}

bool Dungeon::check_collisions(Rectangle rec) {
    bool ok = false;

    rooms.walk([&](auto node, const auto& nodes) -> int {
        Rectangle main = nodes[node].value.rectangle;

        vector<Rectangle> recs;
        for (const auto& i : nodes[node].points_to) {
            const auto& recc = nodes[i].value.rectangle;

            if (CheckCollisionRecs(rec, { recc.x + rec.width, recc.y + rec.height, recc.width - rec.width*2, recc.height - rec.height*2 })) {
                ok = true;
                current_room = i;
                return 1;
            }

            recs.push_back(recc);
        }

        std::sort(recs.begin(), recs.end(), [&main](auto& rec1, auto& rec2) -> bool {
            int dir1 = 0;
            int dir2 = 0;

            if (main.y == (rec1.y + rec1.height)) dir1 = 0;
            else if (main.x == (rec1.x + rec1.width)) dir1 = 3;
            else if ((main.y + main.height) == rec1.y) dir1 = 2;
            else dir1 = 1;

            if (main.y == (rec2.y + rec2.height)) dir2 = 0;
            else if (main.x == (rec2.x + rec2.width)) dir2 = 3;
            else if ((main.y + main.height) == rec2.y) dir2 = 2;
            else dir2 = 1;

            return dir1 <= dir2;
        });

        auto main_verts = get_verts(main);
        main_verts.push_back({ main.x, main.y });
        std::reverse(main_verts.begin(), main_verts.end());

        vector<Vector2> verts{main_verts.back()};
        main_verts.pop_back();


        int side = 0;
        std::size_t i = 0;
        while (i < recs.size()) {
            auto vs = get_verts(recs[i]);

            switch (side) {
            case 0:
                if (main.y == vs[3].y) {
                    std::rotate(vs.begin(), vs.begin()+3, vs.end());
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                }

                verts.push_back(main_verts.back());
                main_verts.pop_back();
                side++;
                break;
            case 1: {
                if ((main.x + main.width) == vs[0].x) {
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                }

                verts.push_back(main_verts.back());
                main_verts.pop_back();
                side++;
                break;
            }
            case 2:
                if ((main.y + main.height) == vs[0].y) {
                    std::rotate(vs.begin(), vs.begin()+1, vs.end());
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                    break;
                }
                verts.push_back(main_verts.back());
                main_verts.pop_back();
                side++;
                break;
            case 3:
                if (main.x == vs[1].x) {
                    std::rotate(vs.begin(), vs.begin()+2, vs.end());
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                }
                verts.push_back(main_verts.back());
                main_verts.pop_back();
                side++;
                break;
            }

            if (side == 4) break;
        }
        std::reverse(main_verts.begin(), main_verts.end());
        verts.insert(verts.end(), main_verts.begin(), main_verts.end());

        // printf("VERTS:\n");
        // for (std::size_t o = 0; o < verts.size(); o++) {
        //     printf("%ld: %f %f\n", o, verts[o].x, verts[o].y);
        // }
        // printf("\n");

        bool pos_bad = false;
        for (const auto& pos : get_verts(rec)) {
            if (!CheckCollisionPointPoly(pos, &verts[0], verts.size())) {
                pos_bad = true;
                break;
            }
        }

        if (!pos_bad) ok = true;

        return 1;
    }, current_room);

    return !ok;
}

Dungeon::~Dungeon() {
    for (auto& t : textures) {
        UnloadTexture(t);
    }
}
