#pragma once

#include <raylib.h>
#include <string>

using std::string;

class AbilityInfo {
public:
    string name;
    int damage;
    int speed;
    int cooldown;
    Texture2D icon;
    Texture2D texture;

    AbilityInfo(string name, int damage, int speed, int cooldown, const char* icon_path, const char* texture_path)
        : name(name), damage(damage), speed(speed), cooldown(cooldown), icon(LoadTexture(icon_path)), texture(LoadTexture(texture_path)) {};
    ~AbilityInfo();
};

class Ability {
public:
    int last_used;
    AbilityInfo& ability;

    Ability(AbilityInfo& ability) : last_used(-ability.cooldown), ability(ability) {};
};
