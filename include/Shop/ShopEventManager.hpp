#ifndef SHOP_EVENT_MANAGER_HPP
#define SHOP_EVENT_MANAGER_HPP

#include "Character/Player.hpp"
#include "NPC/NPC.hpp"
#include "Shop/Shop.hpp"
#include "Shop/ShopScene.hpp"

class ShopEventManager {
public:
    static void OpenShop(NPC::Type type, Player& player, ShopScene& shopScene);
};

#endif
