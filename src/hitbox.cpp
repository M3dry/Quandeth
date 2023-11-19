#include <raylib.h>
#include <vector>

#include "hitbox.hpp"
#include "util.hpp"

using std::vector;

void Hitbox::update_outer_points() {
    vector<Vector2> points{};
    points.resize(recs.size()*4);

    for (const auto& r : recs) {
        auto vs = util::get_verts(r);
        points.insert(points.end(), vs.begin(), vs.end());
    }


    vector<Vector2> points2{};
    points2.resize(points.size());

    for (std::size_t i = 0; i < points.size(); i++) {
        for (const auto& r : recs) {

        }
    }

    outer_points = points;
    // return points2;
}

vector<Vector2> Hitbox::points(Vector2 relative_to) {
    vector<Vector2> ret{};
    ret.resize(outer_points.size());

    for (const auto& p : outer_points) {
        ret.push_back({ p.x + relative_to.x, p.y + relative_to.y });
    }

    return ret;
}
