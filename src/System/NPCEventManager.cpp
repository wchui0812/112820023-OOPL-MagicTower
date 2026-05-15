#include "System/NPCEventManager.hpp"

#include "System/ShopEventManager.hpp"
#include "Util/Logger.hpp"

namespace {
    const char* kFairyPortrait =    RESOURCE_DIR "/Image/Character/NPC/Fairy1.png";
    const char* kElderPortrait =    RESOURCE_DIR "/Image/Character/NPC/Elder1.png";
    const char* kMerchantPortrait = RESOURCE_DIR "/Image/Character/NPC/Merchant1.png";
    const char* kThiefPortrait =    RESOURCE_DIR "/Image/Character/NPC/Thief1.png";
    const char* kPrincessPortrait = RESOURCE_DIR "/Image/Character/NPC/Princess1.png";
    const char* kHeroPortrait =     RESOURCE_DIR "/Image/Character/Player/player_d1.png";
    const char* kShopBossPortrait = RESOURCE_DIR "/Image/Character/Shop/ShopA2.png";
}

void NPCEventManager::DispatchEvent(
    NPC::Type type,
    Player& player,
    Map& map,
    std::shared_ptr<NPC> npc,
    NPCDialog& npcDialog,
    ShopScene& shopScene,
    RewardMessage& rewardMessage
) {
    switch (type) {
        case NPC::Type::FAIRY1:
            HandleFairy1Event(player, map, npc, npcDialog);
            break;
        case NPC::Type::FAIRY2:
            HandleFairy2Event(player, map, npc, npcDialog);
            break;
        case NPC::Type::ELDER1:
            HandleElder1Event(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::ELDER2:
            HandleElder2Event(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::ELDER3:
            HandleElder3Event(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::MERCHANT1:
            HandleMerchant1Event(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::MERCHANT2:
            HandleMerchant2Event(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::THIEF:
            HandleThiefEvent(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::PRINCESS:
            HandlePrincessEvent(player, map, npcDialog);
            break;
        case NPC::Type::SHOP_1:
            HandleShop1IntroEvent(player, npcDialog, shopScene);
            break;
        case NPC::Type::SHOP_2:
            HandleShop2Event(player, shopScene);
            break;
        case NPC::Type::ELDER_SHOP_1:
            HandleElderShop1Event(player, shopScene);
            break;
        case NPC::Type::ELDER_SHOP_2:
            HandleElderShop2Event(player, shopScene);
            break;
        case NPC::Type::MERCHANT_SHOP_1:
            HandleMerchantShop1Event(player, shopScene);
            break;
        case NPC::Type::MERCHANT_SHOP_2:
            HandleMerchantShop2Event(player, shopScene);
            break;
        default:
            LOG_INFO("This NPC has no event.");
            break;
    }
}

void NPCEventManager::HandleFairy1Event(Player& player,Map& map,std::shared_ptr<NPC> npc,NPCDialog& npcDialog) {
    npcDialog.Start({
        {"勇者", "......", kHeroPortrait},
        {"仙子", "你醒了!", kFairyPortrait},
        {"勇者", "你是誰，我在哪裡?", kHeroPortrait},
        {"仙子", "我是這裡的仙子，剛才你被這裡的小怪打昏了", kFairyPortrait},
        {"勇者", "劍!劍!我的劍呢?", kHeroPortrait},
        {"仙子", "你的劍被他們搶走了，我只來得及將你救出來而已", kFairyPortrait},
        {"勇者", "那公主呢?我是來救公主的", kHeroPortrait},
        {"仙子", "公主還在裡面，你這樣進去是打不過裡面的小怪的", kFairyPortrait},
        {"勇者", "哪我怎麼辦，我答應了國王一定要把公主救出來的，那我現在應該怎麼辦呀?", kHeroPortrait},
        {"仙子", "放心吧，我把我的力量借給你，你就可以打贏那些小怪了。不過，你得先幫我去找一樣東西，找到了再來這裡找我", kFairyPortrait},
        {"勇者", "找東西?什麼東西?", kHeroPortrait},
        {"仙子", "是一個十字架，中間有一顆紅色的寶石", kFairyPortrait},
        {"勇者", "那個東西有什麼用嗎", kHeroPortrait},
        {"仙子", "我本是這塔的守護者，可不久之前，從北方來了一批惡魔，他們佔領了這座塔，並將我的魔力封在了這個十字架裡面", kFairyPortrait},
        {"仙子", "如果你能將它帶出來，那我的魔力便會慢慢的恢復，到時候我就可以把力量借給你去救出公主了", kFairyPortrait},
        {"仙子", "要記住，只有用我的魔力才可以打開21層的門", kFairyPortrait},
        {"勇者", "好吧，我試試看", kHeroPortrait},
        {"仙子", "剛才我去看過了，你的劍被放在三樓，你的盾在五樓，而那個十字架被放在七樓，要到七樓，你得先取回你的劍和盾", kFairyPortrait},
        {"仙子", "另外，在塔裡的其他樓層，還有一些存放了好幾百年的寶劍和寶物，如果得到它們，對於你將會有很大的幫助", kFairyPortrait},
        {"仙子", "我這裡有三把鑰匙，你先拿去，在塔裡還有很多這樣的鑰匙，你一定要珍惜使用", kFairyPortrait},
        {"仙子", "勇敢地去吧，勇士!", kFairyPortrait},
    }, [&player, &map, npc]() {

        map.MoveNPC(npc, -1, 0);

        player.m_YellowKeys += 1;
        player.m_BlueKeys += 1;
        player.m_RedKeys += 1;
    });
}

void NPCEventManager::HandleFairy2Event(Player& player,Map& map,std::shared_ptr<NPC> npc,NPCDialog& npcDialog) {
    npcDialog.Start({
        {"勇者", "......", kHeroPortrait},
        {"仙子", "你醒了!", kFairyPortrait},
        {"勇者", "你是誰，我在哪裡?", kHeroPortrait},
        {"仙子", "我是這裡的仙子，剛才你被這裡的小怪打昏了", kFairyPortrait},
        {"勇者", "劍!劍!我的劍呢?", kHeroPortrait},
        {"仙子", "你的劍被他們搶走了，我只來得及將你救出來而已", kFairyPortrait},
        {"勇者", "那公主呢?我是來救公主的", kHeroPortrait},
        {"仙子", "公主還在裡面，你這樣進去是打不過裡面的小怪的", kFairyPortrait},
        {"勇者", "哪我怎麼辦，我答應了國王一定要把公主救出來的，那我現在應該怎麼辦呀?", kHeroPortrait},
        {"仙子", "放心吧，我把我的力量借給你，你就可以打贏那些小怪了。不過，你得先幫我去找一樣東西，找到了再來這裡找我", kFairyPortrait},
        {"勇者", "找東西?什麼東西?", kHeroPortrait},
        {"仙子", "是一個十字架，中間有一顆紅色的寶石", kFairyPortrait},
        {"勇者", "那個東西有什麼用嗎", kHeroPortrait},
        {"仙子", "我本是這塔的守護者，可不久之前，從北方來了一批惡魔，他們佔領了這座塔，並將我的魔力封在了這個十字架裡面", kFairyPortrait},
        {"仙子", "如果你能將它帶出來，那我的魔力便會慢慢的恢復，到時候我就可以把力量借給你去救出公主了", kFairyPortrait},
        {"仙子", "要記住，只有用我的魔力才可以打開21層的門", kFairyPortrait},
        {"勇者", "好吧，我試試看", kHeroPortrait},
        {"仙子", "剛才我去看過了，你的劍被放在三樓，你的盾在五樓，而那個十字架被放在七樓，要到七樓，你得先取回你的劍和盾", kFairyPortrait},
        {"仙子", "另外，在塔裡的其他樓層，還有一些存放了好幾百年的寶劍和寶物，如果得到它們，對於你將會有很大的幫助", kFairyPortrait},
        {"仙子", "我這裡有三把鑰匙，你先拿去，在塔裡還有很多這樣的鑰匙，你一定要珍惜使用", kFairyPortrait},
        {"仙子", "勇敢地去吧，勇士!", kFairyPortrait},
    }, [&player, &map, npc]() {

        map.MoveNPC(npc, -1, 0);

        player.m_YellowKeys += 1;
        player.m_BlueKeys += 1;
        player.m_RedKeys += 1;
    });
}

void NPCEventManager::HandleElder1Event(Player& player,Map& map,NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)map;

    npcDialog.Start({
        {"勇者", "您已經得救了！", kHeroPortrait},
        {"神秘老人", "喔，我的孩子，真是太感謝你了！", kElderPortrait},
        {"神秘老人", "這個地方又髒又壞，我真的是快待不下去了。", kElderPortrait},
        {"勇者", "快走吧，我還得救被關在這裡的公主。", kHeroPortrait},
        {"神秘老人", "這個東西就送給你吧，這還是我年輕的時候用過的。", kElderPortrait},
    }, [&player, &rewardMessage]() {
        player.m_Atk += 30;
        rewardMessage.ShowItem("獲得鋼劍! 攻擊力 +30");
    });
}

void NPCEventManager::HandleElder2Event(Player& player,Map& map,NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)map;

    npcDialog.Start({
        {"神秘老人", "你好，勇敢的孩子，你終於來到這裡了。", kElderPortrait},
        {"神秘老人", "我將給你一個非常好的寶物，它可以使你的攻擊力提升 120 點，但這必須用你的 500 點經驗來交換，考慮一下吧！", kElderPortrait},
        {"勇者", "好吧，那就將寶物給我吧！", kHeroPortrait},
    }, [&player, &rewardMessage]() {
        const int costExp = 500;

        if (player.m_Exp < costExp) {
            rewardMessage.ShowItem("經驗不足，需要 500 點經驗!");
            return;
        }

        player.m_Exp -= costExp;
        player.m_Atk += 120;
        rewardMessage.ShowItem("獲得聖光劍! 攻擊力 +120");
    });
}

void NPCEventManager::HandleElder3Event(Player& player,Map& map,NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)map;

    npcDialog.Start({
        {"勇者", "您已經得救了！", kHeroPortrait},
        {"神秘老人", "喔，我的孩子，真是太感謝你了！", kElderPortrait},
        {"神秘老人", "這個地方又髒又壞，我真的是快待不下去了。", kElderPortrait},
        {"勇者", "快走吧，我還得救被關在這裡的公主。", kHeroPortrait},
        {"神秘老人", "這個東西就送給你吧，這還是我年輕的時候用過的。", kElderPortrait},
    }, [&player, &rewardMessage]() {
        player.m_Atk += 30;
        rewardMessage.ShowItem("獲得鋼劍! 攻擊力 +30");
    });
}

void NPCEventManager::HandleMerchant1Event(Player& player,Map& map,NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)map;

    npcDialog.Start({
        {"勇者", "您已經得救了！", kHeroPortrait},
        {"商人", "喔，是嘛，真是太感謝你了！", kMerchantPortrait},
        {"商人", "我是個商人，不知為什麼被抓到這裡來了。", kMerchantPortrait},
        {"勇者", "快走吧，現在你已經自由了。", kHeroPortrait},
        {"商人", "那這個東西就給你吧，本來我是準備賣錢的。", kMerchantPortrait},
    }, [&player, &rewardMessage]() {
        player.m_Def += 30;
        rewardMessage.ShowItem("獲得鋼盾! 防禦力 +30");
    });
}

void NPCEventManager::HandleMerchant2Event(Player& player,Map& map,NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)map;

    npcDialog.Start({
        {"商人", "啊哈，歡迎你的到來！我這裡有一件對你來說非常好的寶物，只要你出得起錢，我就賣給你。", kMerchantPortrait},
        {"勇者", "什麼寶物？要多少錢？", kHeroPortrait},
        {"商人", "是這個遊戲最好的盾牌，防禦力可以可以增加 120 點，而你只要出 500 個金幣就可以買下。", kMerchantPortrait},
        {"商人", "怎麼樣？你有 500 個金幣嗎？", kMerchantPortrait},
        {"勇者", "我有 500 個金幣。", kHeroPortrait},
        {"商人", "好，成交！", kMerchantPortrait},
    }, [&player, &rewardMessage]() {
        const int costCoin = 500;

        if (player.m_Coin < costCoin) {
            rewardMessage.ShowItem("金幣不足，需要 500 個金幣!");
            return;
        }

        player.m_Coin -= costCoin;
        player.m_Def += 120;
        rewardMessage.ShowItem("獲得星光盾! 防禦力 +120");
    });
}

void NPCEventManager::HandleThiefEvent(Player& player, Map& map, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    (void)player;

    npcDialog.Start({
        {"勇者", "您已經得救了！", kHeroPortrait},
        {"小偷", "啊，那真是太好了，我又可以在這裡面尋寶了！", kThiefPortrait},
        {"小偷", "喔，還沒有自我介紹，我叫杰克，是這附近有名的小偷，什麼金銀財寶我樣樣都偷過。", kThiefPortrait},
        {"勇者", "快走吧，外面還有很多怪物，我可能顧不上你。", kHeroPortrait},
        {"小偷", "不，不，不會有事的。快說吧，叫我做什麼？", kThiefPortrait},
        {"勇者", "你會開門嗎？", kHeroPortrait},
        {"小偷", "那當然。", kThiefPortrait},
        {"勇者", "那就請你幫我打開第二層的門吧！", kHeroPortrait},
        {"小偷", "那個簡單，不過，如果你能幫我找到一把嵌了紅寶石的鐵鎬，我還能幫你打通第十八層的路。", kThiefPortrait},
        {"勇者", "嵌了紅寶石的鐵鎬？好吧，我幫你找找。", kHeroPortrait},
        {"小偷", "非常地感謝。一會我便會將第二層的門打開。", kThiefPortrait},
    }, [&map, &rewardMessage]() {
        map.RemoveTileAtLevel(2, 6, 1);
        rewardMessage.ShowItem("小偷打開了第二層的綠色門!");
    });
}

void NPCEventManager::HandlePrincessEvent(Player& player, Map& map, NPCDialog& npcDialog) {
    npcDialog.Start({
        {"公主", "勇者，你終於來了。", kPrincessPortrait},
        {"勇者", "我會帶你離開這座塔。", kHeroPortrait},
        {"公主", "請小心，越往上怪物會越強。", kPrincessPortrait}
    });
}

void NPCEventManager::HandleShop1IntroEvent(
    Player& player,
    NPCDialog& npcDialog,
    ShopScene& shopScene
) {
    npcDialog.Start({
        {"商店老闆", "嗨，你好，英雄的勇士，這裡是怪物商店，告訴你一些操作方法： 使用上下鍵來選擇項目，按 Space或 Enter 可以用來確認你的選擇！", kShopBossPortrait},
        //{"商店老闆", "使用上下鍵選擇項目，按 Space 或 Enter 可以用來確認你的選擇！", kShopBossPortrait},
        {"商店老闆", "同時在商人或神秘老人處進行交易也是一樣的操作方法!知道了嗎？勇士！", kShopBossPortrait},
    }, [&player, &shopScene]() {
        ShopEventManager::OpenShop(NPC::Type::SHOP_1, player, shopScene);
    });
}

void NPCEventManager::HandleShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::SHOP_2, player, shopScene);
}

void NPCEventManager::HandleElderShop1Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::ELDER_SHOP_1, player, shopScene);
}

void NPCEventManager::HandleElderShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::ELDER_SHOP_2, player, shopScene);
}

void NPCEventManager::HandleMerchantShop1Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::MERCHANT_SHOP_1, player, shopScene);
}

void NPCEventManager::HandleMerchantShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::MERCHANT_SHOP_2, player, shopScene);
}
