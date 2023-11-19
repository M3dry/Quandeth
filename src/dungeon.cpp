#include <raylib.h>
#include <utility>
#include <vector>
#include <tuple>
#include <stdio.h>

#include "dungeon.hpp"
#include "graph.hpp"
#include "util.hpp"

static bool recs_sort(const Rectangle& main, const Rectangle& rec1, const Rectangle& rec2) {
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

    if (dir1 == dir2) {
        switch (dir1) {
        case 0:
            return rec1.x < rec2.x;
        case 1:
            return rec1.y < rec2.y;
        case 2:
            return rec1.y < rec2.y;
        case 3:
            return rec1.x < rec2.x;
        default:
            std::unreachable();
        }
    } else return dir1 < dir2;
}

Dungeon::Dungeon(Vector2 spawn, vector<graph::Node<Room>> rooms, std::size_t spawn_room, vector<const char *> texture_paths,
                 Color room_floor, Color hallway_floor, Color border) :
            spawn((Vector2){ rooms[spawn_room].value.rectangle.x + spawn.x, rooms[spawn_room].value.rectangle.y + spawn.y }),
            rooms(rooms, spawn_room),
            room_floor(room_floor), hallway_floor(hallway_floor), border(border) {
    textures.reserve(texture_paths.size());
    for (std::size_t i = 0; i < texture_paths.size(); i++) {
        textures[i] = LoadTexture(texture_paths[i]);
    }
};

void Dungeon::draw(bool borders, float scale) {
    if (borders) {
        rooms.walk([this, &scale](auto node, const auto& nodes) -> int {
            auto rec = util::scale_rec(nodes[node].value.rectangle, scale);
            auto b_scale = 5*scale;

            DrawRectangle(rec.x - b_scale, rec.y - b_scale, rec.width + 2*b_scale, rec.height + 2*b_scale, border);

            return 0;
        });
    }

    rooms.walk([&](auto node, const auto& nodes) -> int {
        auto color = nodes[node].value.type == Room::TRoom ? room_floor : hallway_floor;
        auto rec = nodes[node].value.rectangle;

        DrawRectangleRec(util::scale_rec(rec, scale), color);

        for (const auto& [v, i] : nodes[node].value.obstacles) {
            auto inside = util::inside(util::scale_rec(nodes[node].value.rectangle, scale), { v.x, v.y, textures[i].width*scale, textures[i].height*scale });
            if (inside) {
                auto ins = *inside;
                DrawTextureRec(textures[i], (Rectangle){ 0, 0, ins.width, ins.height }, (Vector2){ ins.x, ins.y }, WHITE);
            }
        }

        return 0;
    });
}

bool Dungeon::check_collisions(Rectangle rec, std::size_t& room) {
    bool ok = false;

    rooms.walk([&](auto node, const auto& nodes) -> int {
        Rectangle main = nodes[node].value.rectangle;

        vector<Rectangle> recs;
        recs.reserve(nodes[node].points_to.size());
        for (const auto& i : nodes[node].points_to) {
            const auto& recc = nodes[i].value.rectangle;

            if (CheckCollisionRecs(rec, { recc.x + rec.width, recc.y + rec.height, recc.width - rec.width*2, recc.height - rec.height*2 })) {
                ok = true;
                room = i;
                return 1;
            }

            recs.push_back(recc);
        }

        std::sort(recs.begin(), recs.end(), [&main](auto& rec1, auto& rec2) -> bool {
            return recs_sort(main, rec1, rec2);
        });

        auto main_verts = util::get_verts(main);
        main_verts.push_back({ main.x, main.y });
        std::reverse(main_verts.begin(), main_verts.end());

        vector<Vector2> verts{main_verts.back()};
        main_verts.pop_back();


        int side = 0;
        std::size_t i = 0;
        while (i < recs.size()) {
            auto vs = util::get_verts(recs[i]);

            switch (side) {
            case 0:
                if (main.y == vs[3].y) {
                    std::rotate(vs.begin(), vs.begin()+3, vs.end());
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                    break;
                }

                verts.push_back(main_verts.back());
                main_verts.pop_back();
                side++;
                break;
            case 1: {
                if ((main.x + main.width) == vs[0].x) {
                    verts.insert(verts.end(), vs.begin(), vs.end());
                    i++;
                    break;
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
                    break;
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

        bool pos_bad = false;
        for (const auto& pos : util::get_verts(rec)) {
            if (!CheckCollisionPointPoly(pos, &verts[0], verts.size())) {
                pos_bad = true;
                break;
            }
        }

        if (!pos_bad) ok = true;

        return 1;
    }, room);

    return !ok;
}

Dungeon::~Dungeon() {
    for (auto& t : textures) {
        UnloadTexture(t);
    }
}
