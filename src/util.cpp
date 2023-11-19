#include <raylib.h>
#include <optional>
#include <vector>

#include "util.hpp"

namespace util {
    std::optional<Rectangle> inside(Rectangle into, Rectangle what) {
        auto rec = (Rectangle){ .x = into.x + what.x, .y = into.x + what.y };

        if (rec.x > (into.x + into.width) || rec.y > (into.y + into.height)) return {};
        auto max_width = (into.x + into.width) - rec.x;
        auto max_height = (into.y + into.height) - rec.y;
        rec.width = max_width >= what.width ? what.width : max_width;
        rec.height = max_height >= what.height ? what.height : max_height;

        return rec;
    }

    Vector2 center(Rectangle rec) {
        return (Vector2){ rec.x + rec.width/2, rec.y + rec.height/2 };
    }

    Vector2 center(Rectangle rec, Vector2 dims) {
        return (Vector2){ rec.x + rec.width/2 - dims.x/2, rec.y + rec.height/2 - dims.y/2 };
    }

    Rectangle scale_rec(Rectangle rec, float scale) {
        return { rec.x * scale, rec.y * scale, rec.width * scale, rec.height * scale };
    }

    std::vector<Vector2> get_verts(const Rectangle& rec) {
        std::vector<Vector2> vec{
            { rec.x, rec.y },
            { rec.x + rec.width, rec.y },
            { rec.x + rec.width, rec.y + rec.height },
            { rec.x, rec.y + rec.height }
        };
        return vec;
    }
}
