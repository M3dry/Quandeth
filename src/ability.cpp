#include <raylib.h>
#include <string>

#include "ability.hpp"

AbilityInfo::~AbilityInfo() {
    UnloadTexture(icon);
    UnloadTexture(texture);
}
