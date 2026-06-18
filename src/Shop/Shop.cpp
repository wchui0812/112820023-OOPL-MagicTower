#include "Shop/Shop.hpp"
#include "Character/Player.hpp"

#include <utility>

Shop::Shop(
    std::string title,
    std::string greeting,
    std::string portraitPath,
    std::vector<ShopItem> items
) : m_Title(std::move(title)),
    m_Greeting(std::move(greeting)),
    m_PortraitPath1(std::move(portraitPath)),
    m_PortraitPath2(m_PortraitPath1),
    m_Items(std::move(items)) {}

Shop::Shop(
    std::string title,
    std::string greeting,
    std::string portraitPath1,
    std::string portraitPath2,
    std::vector<ShopItem> items
) : m_Title(std::move(title)),
    m_Greeting(std::move(greeting)),
    m_PortraitPath1(std::move(portraitPath1)),
    m_PortraitPath2(std::move(portraitPath2)),
    m_Items(std::move(items)) {}

ShopPurchaseResult Shop::Purchase(Player& player, int itemIndex) const {
    if (itemIndex < 0 || itemIndex >= static_cast<int>(m_Items.size())) {
        return {false, ""};
    }

    const auto& item = m_Items[itemIndex];
    if (item.exitsShop) {
        return {true, ""};
    }

    int* currency = nullptr;
    std::string currencyName;
    if (item.costType == ShopCostType::EXP) {
        currency = &player.m_Exp;
        currencyName = "經驗值";
    } else if (item.costType == ShopCostType::COIN) {
        currency = &player.m_Coin;
        currencyName = "金幣";
    }

    if (currency && *currency < item.cost) {
        return {false, "你的" + currencyName + "不足，還不能購買這個項目。"};
    }

    if (item.yellowKeyDelta < 0 && player.m_YellowKeys < -item.yellowKeyDelta) {
        return {false, "你的黃鑰匙不足，無法販賣。"};
    }
    if (item.blueKeyDelta < 0 && player.m_BlueKeys < -item.blueKeyDelta) {
        return {false, "你的藍鑰匙不足，無法販賣。"};
    }
    if (item.redKeyDelta < 0 && player.m_RedKeys < -item.redKeyDelta) {
        return {false, "你的紅鑰匙不足，無法販賣。"};
    }

    if (currency) {
        *currency -= item.cost;
    }

    player.m_Level += item.levelDelta;
    player.m_Hp += item.hpDelta;
    player.m_Atk += item.atkDelta;
    player.m_Def += item.defDelta;
    player.m_Coin += item.coinDelta;
    player.m_YellowKeys += item.yellowKeyDelta;
    player.m_BlueKeys += item.blueKeyDelta;
    player.m_RedKeys += item.redKeyDelta;

    return {false, item.successMessage.empty() ? "交易完成。" : item.successMessage};
}
