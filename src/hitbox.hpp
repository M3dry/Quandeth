#pragma once

#include <raylib.h>
#include <vector>

using std::vector;

class Hitbox {
    private:
        vector<Vector2> outer_points;

        void update_outer_points();

    public:
        vector<Rectangle> recs;

        Hitbox() : outer_points({}), recs({}) {};
        Hitbox(vector<Rectangle> recs) : outer_points({}), recs(recs) {
            update_outer_points();
        };
        vector<Vector2> points(Vector2 relative_to);
};
