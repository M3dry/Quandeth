#include <iostream>
#include <raylib.h>
#include <vector>
#include <span>
#include <optional>
#include <memory>
#include <any>
#include <functional>
#include <cassert>

#include "quant.hpp"

using std::vector;
using std::span;
using std::unique_ptr;
using std::optional;
using std::any;
using std::function;

void quant_test() {
    Quant quant("Fireball", 100, {
        ModifierType(ModifierType::Category::General, ModifierType::General::Damage),
        ModifierType(ModifierType::Category::General, ModifierType::General::Cooldown),
        ModifierType(ModifierType::Category::QuantMorphing, ModifierType::QuantMorphing::AOE),
    });
    QuantHole quanthole(quant, std::make_unique<clocks::OPNeedName>());

    std::cout << quanthole.cooldown << std::endl;
    std::cout << quanthole.root.modifiers.size() << std::endl;
}

namespace clocks {
    int BadNeedName::delay(const QuantNode& tree) {
        return 69;
    }

    int OPNeedName::delay(const QuantNode& tree) {
        return tree.quant.cooldown;
    }
}

Quant::Quant(string name, int cooldown, vector<ModifierType> modifier_types)
    : name(name), cooldown(cooldown), modifier_types_vec(std::move(modifier_types)) {
    this->modifier_types = modifier_types_vec;
};

QuantNode::QuantNode(Quant quant) : quant(quant) {
    auto size = quant.modifier_types.size();
    modifiers_ptr = std::make_unique<void*[]>(size);
    modifiers = { modifiers_ptr.get(), size };

    apply_modifiers();
};

void QuantNode::apply_modifiers() {

};

QuantHole::QuantHole(Quant quant, unique_ptr<clocks::ClockI> clock_) : root(quant), clock(std::move(clock_)) {
    cooldown = clock->delay(root);
};
