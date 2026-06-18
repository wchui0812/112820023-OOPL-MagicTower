#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "System/BattleScene.hpp"
#include "System/RewardMessage.hpp"
#include "NPC/NPCDialog.hpp"
#include "Shop/ShopScene.hpp"

#include <glm/vec2.hpp>

class CollisionHandler {
public:

    static bool HandleCollision(
        Player& player,
        Map& map,
        const glm::vec2& targetPos,
        BattleScene& battleScene,
        RewardMessage& rewardMessage,
        NPCDialog& npcDialog,
        ShopScene& shopScene
        );
};

#endif
