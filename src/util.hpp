#pragma once

#include <raylib.h>
#include <optional>
#include <vector>

namespace util {
    std::optional<Rectangle> inside(Rectangle into, Rectangle what);
    Vector2 center(Rectangle rec);
    Vector2 center(Rectangle rec, Vector2 dims);
    Rectangle scale_rec(Rectangle rec, float scale);
    std::vector<Vector2> get_verts(const Rectangle& rec);
}
