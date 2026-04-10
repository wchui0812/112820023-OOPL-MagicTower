#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include <glm/vec2.hpp>

class CollisionHandler {
public:
    // 回傳值代表「是否允許玩家踏上那一格」
    static bool HandleCollision(Player& player, Map& map, const glm::vec2& targetPos);
};

#endif