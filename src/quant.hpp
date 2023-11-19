#pragma once

#include <raylib.h>
#include <vector>
#include <span>
#include <optional>
#include <memory>
#include <functional>
#include <variant>
#include <string>
#include <stdio.h>

using std::vector;
using std::span;
using std::unique_ptr;
using std::optional;
using std::function;
using std::variant;
using std::string;

class QuantNode;

void quant_test();

namespace modifiers {
    enum class Pos {
        PlayerToMouse,
        PlayerToNearestEnemy,
        Mouse, // AOE only
    };
}

class ModifierType {
public:
    enum class Category {
        General,
        QuantMorphing,
        AOE,
        Beam,
        Projectile
    };

    enum class General {
        Damage,
        Cooldown,
        Pos,
    };

    enum class QuantMorphing {
        AOE,
        Projectile,
        Beam,
    };

    enum class AOE {
        EffectTime,
        Radius,
    };

    enum class Beam {
        Girth,
        FocalPoint,
        FlowRate,
    };

    enum class Projectile {
        Speed,
        Size,
    };

    typedef variant<ModifierType::General, ModifierType::QuantMorphing, ModifierType::AOE, ModifierType::Beam, ModifierType::Projectile> Type;

    Category category;
    Type type;

    ModifierType(Category category, Type type) : category(category), type(type) {};
};

namespace clocks {
    class ClockI {
    public:
        virtual int delay(const QuantNode& tree) = 0;
    };

    // Default
    class BadNeedName : public ClockI {
    public:
        int delay(const QuantNode& tree);
    };

    class OPNeedName : public ClockI {
    public:
        int delay(const QuantNode& tree);
    };
}

class Quant {
public:
    string name;
    int cooldown;

    span<ModifierType> modifier_types;

    Quant(string name, int cooldown, vector<ModifierType> modifier_types);

private:
    vector<ModifierType> modifier_types_vec;
};

class QuantNode {
public:
    Quant quant;

    // types, by which to cast, come from `quant.modifier_types`
    span<void*> modifiers;

    QuantNode(Quant quant);
private:
    unique_ptr<void*[]> modifiers_ptr;

    void apply_modifiers();
};

class QuantLink {

};

class QuantHole {
public:
    QuantNode root;
    int cooldown;
    // tree modifiers
    unique_ptr<clocks::ClockI> clock;

    QuantHole(Quant quant, unique_ptr<clocks::ClockI> clock);
};
