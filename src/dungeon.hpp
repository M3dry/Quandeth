#pragma once

#include <raylib.h>
#include <vector>
#include <tuple>

#include "graph.hpp"

using std::vector;
using std::tuple;
using std::size_t;

class Room {
public:
    typedef enum { TRoom, THallway } Type;

    Type type;
    Rectangle rectangle;
    vector<tuple<Vector2, size_t>> obstacles; // position is relative to the `rectangle`

    Room(Type type, Rectangle rectangle, vector<tuple<Vector2, size_t>> obstacles = {}) :
        type(type), rectangle(rectangle), obstacles(obstacles) {};
};

class Dungeon {
public:
    Vector2 spawn;
    vector<Texture2D> textures;
    graph::Graph<Room> rooms;
    Color room_floor;
    Color hallway_floor;
    Color border;

    Dungeon(Vector2 spawn, vector<graph::Node<Room>> rooms, size_t spawn_room, vector<const char *> texture_paths = {},
            Color room_floor = GetColor(0x292d3eff), Color hallway_floor = GetColor(0x292d3eff), Color border = WHITE);
    void draw(bool borders = true, float scale = 1);
    bool check_collisions(Rectangle rec, size_t& room);
    ~Dungeon();
};
