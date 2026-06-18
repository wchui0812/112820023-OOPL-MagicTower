#include "Shop/ShopEventManager.hpp"

namespace {
    ShopItem ExitItem() {
        return {"離開", 0, ShopCostType::NONE, 0, 0, 0, 0, true, ""};
    }

    Shop MakeMonsterShopGuide1() {
        return Shop(
            "金幣商店",
            "想要增加你的能力嗎？如果你有25個金幣，你可以任意選擇一項：",
            RESOURCE_DIR "/Image/Character/Shop/ShopA2.png",
            RESOURCE_DIR "/Image/Character/Shop/ShopB2.png",
            {
                {"增加 800 點生命", 25, ShopCostType::COIN, 800, 0, 0, 0, false, "生命增加了 800 點。"},
                {"增加 4 點攻擊", 25, ShopCostType::COIN, 0, 4, 0, 0, false, "攻擊力提升了 4 點。"},
                {"增加 4 點防禦", 25, ShopCostType::COIN, 0, 0, 4, 0, false, "防禦力提升了 4 點。"},
                ExitItem(),
            }
        );
    }

    Shop MakeMonsterShopGuide2() {
        return Shop(
            "金幣商店",
            "想要增加你的能力嗎？如果你有100個金幣，你可以任意選擇一項：",
            RESOURCE_DIR "/Image/Character/Shop/ShopA2.png",
            RESOURCE_DIR "/Image/Character/Shop/ShopB2.png",
            {
                {"增加 4000 點生命", 100, ShopCostType::COIN, 4000, 0, 0, 0, false, "生命增加了 4000 點。"},
                {"增加 20 點攻擊", 100, ShopCostType::COIN, 0, 20, 0, 0, false, "攻擊力提升了 20 點。"},
                {"增加 20 點防禦", 100, ShopCostType::COIN, 0, 0, 20, 0, false, "防禦力提升了 20 點。"},
                ExitItem(),
            }
        );
    }

    Shop MakeExpShop1() {
        return Shop(
            "經驗老人",
            "你好，英雄的人類，只要你有足夠的經驗，我就可以讓你變得更強大：",
            RESOURCE_DIR "/Image/Character/Shop/Elder1.png",
            RESOURCE_DIR "/Image/Character/Shop/Elder2.png",
            {
                {"提升一級（需要 100 點）", 100, ShopCostType::EXP, 1000, 7, 7, 1, false, "等級提升了。"},
                {"增加攻擊 5（需要 30 點）", 30, ShopCostType::EXP, 0, 5, 0, 0, false, "攻擊力提升了 5 點。"},
                {"增加防禦 5（需要 30 點）", 30, ShopCostType::EXP, 0, 0, 5, 0, false, "防禦力提升了 5 點。"},
                ExitItem(),
            }
        );
    }

    Shop MakeExpShop2() {
        return Shop(
            "經驗老人",
            "你好，英雄的人類，只要你有足夠的經驗，我就可以讓你變得更強大：",
            RESOURCE_DIR "/Image/Character/Shop/Elder1.png",
            RESOURCE_DIR "/Image/Character/Shop/Elder2.png",
            {
                {"提升三級（需要 270 點）", 270, ShopCostType::EXP, 3000, 21, 21, 3, false, "等級提升了 3 級。"},
                {"增加攻擊 17（需要 95 點）", 95, ShopCostType::EXP, 0, 17, 0, 0, false, "攻擊力提升了 17 點。"},
                {"增加防禦 17（需要 95 點）", 95, ShopCostType::EXP, 0, 0, 17, 0, false, "防禦力提升了 17 點。"},
                ExitItem(),
            }
        );
    }

    Shop MakeKeyShop1() {
        return Shop(
            "商人",
            "相信你一定有特殊的需要，只要你有金幣，我就可以幫你：",
            RESOURCE_DIR "/Image/Character/Shop/ShopKeeper1.png",
            RESOURCE_DIR "/Image/Character/Shop/ShopKeeper2.png",
            {
                {"購買 1 把黃鑰匙（$ 10）", 10, ShopCostType::COIN, 0, 0, 0, 0, false, "獲得一把黃鑰匙。", 0, 1, 0, 0},
                {"購買 1 把藍鑰匙（$ 50）", 50, ShopCostType::COIN, 0, 0, 0, 0, false, "獲得一把藍鑰匙。", 0, 0, 1, 0},
                {"購買 1 把紅鑰匙（$ 100）", 100, ShopCostType::COIN, 0, 0, 0, 0, false, "獲得一把紅鑰匙。", 0, 0, 0, 1},
                ExitItem(),
            }
        );
    }

    Shop MakeKeyShop2() {
        return Shop(
            "商人",
            "喔，歡迎你的到來，如果你手裡缺少金幣，我可幫你：",
            RESOURCE_DIR "/Image/Character/Shop/ShopKeeper1.png",
            RESOURCE_DIR "/Image/Character/Shop/ShopKeeper2.png",
            {
                {"賣出 1 把黃鑰匙（$ 7）", 0, ShopCostType::NONE, 0, 0, 0, 0, false, "賣出一把黃鑰匙，獲得 7 金幣。", 7, -1, 0, 0},
                {"賣出 1 把藍鑰匙（$ 35）", 0, ShopCostType::NONE, 0, 0, 0, 0, false, "賣出一把藍鑰匙，獲得 35 金幣。", 35, 0, -1, 0},
                {"賣出 1 把紅鑰匙（$ 70）", 0, ShopCostType::NONE, 0, 0, 0, 0, false, "賣出一把紅鑰匙，獲得 70 金幣。", 70, 0, 0, -1},
                ExitItem(),
            }
        );
    }
}

void ShopEventManager::OpenShop(NPC::Type type, Player& player, ShopScene& shopScene) {
    switch (type) {
        case NPC::Type::SHOP_1:
            shopScene.Open(player, MakeMonsterShopGuide1());
            break;
        case NPC::Type::SHOP_2:
            shopScene.Open(player, MakeMonsterShopGuide2());
            break;
        case NPC::Type::ELDER_SHOP_1:
            shopScene.Open(player, MakeExpShop1());
            break;
        case NPC::Type::ELDER_SHOP_2:
            shopScene.Open(player, MakeExpShop2());
            break;
        case NPC::Type::MERCHANT_SHOP_1:
            shopScene.Open(player, MakeKeyShop1());
            break;
        case NPC::Type::MERCHANT_SHOP_2:
            shopScene.Open(player, MakeKeyShop2());
            break;
        default:
            break;
    }
}
