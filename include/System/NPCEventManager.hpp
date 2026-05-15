#ifndef NPC_EVENT_MANAGER_HPP
#define NPC_EVENT_MANAGER_HPP

#include "Character/Player.hpp"
#include "Map/Map.hpp"
#include "NPC/NPC.hpp"
#include "System/NPCDialog.hpp"
#include "System/RewardMessage.hpp"
#include "System/ShopScene.hpp"

class NPCEventManager {
public:
    // 核心函式：根據 NPC 類型觸發事件
    static void DispatchEvent(NPC::Type type, Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog, ShopScene& shopScene,RewardMessage& rewardMessage);

private:
    // 將不同 NPC 的邏輯拆分成私有函式，方便管理
    static void HandleFairy1Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog);
    static void HandleFairy2Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog);
    static void HandleElder1Event(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandleElder2Event(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandleElder3Event(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandleMerchant1Event(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandleMerchant2Event(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandleThiefEvent(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage);
    static void HandlePrincessEvent(Player& player, Map& map, NPCDialog& npcDialog);

    static void HandleShop1IntroEvent(Player& player, NPCDialog& npcDialog, ShopScene& shopScene);
    static void HandleShop2Event(Player& player, ShopScene& shopScene);
    static void HandleElderShop1Event(Player& player, ShopScene& shopScene);
    static void HandleElderShop2Event(Player& player, ShopScene& shopScene);
    static void HandleMerchantShop1Event(Player& player, ShopScene& shopScene);
    static void HandleMerchantShop2Event(Player& player, ShopScene& shopScene);
};

#endif
