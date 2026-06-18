#ifndef SHOP_HPP
#define SHOP_HPP

#include <string>
#include <vector>

class Player;

enum class ShopCostType {
    NONE,
    EXP,
    COIN
};

struct ShopItem {
    std::string text;
    int cost = 0;
    ShopCostType costType = ShopCostType::NONE;
    int hpDelta = 0;
    int atkDelta = 0;
    int defDelta = 0;
    int levelDelta = 0;
    bool exitsShop = false;
    std::string successMessage;
    int coinDelta = 0;
    int yellowKeyDelta = 0;
    int blueKeyDelta = 0;
    int redKeyDelta = 0;
};

struct ShopPurchaseResult {
    bool shouldClose = false;
    std::string message;
};

class Shop {
public:
    Shop() = default;
    Shop(
        std::string title,
        std::string greeting,
        std::string portraitPath,
        std::vector<ShopItem> items
    );
    Shop(
        std::string title,
        std::string greeting,
        std::string portraitPath1,
        std::string portraitPath2,
        std::vector<ShopItem> items
    );

    const std::string& GetTitle() const { return m_Title; }
    const std::string& GetGreeting() const { return m_Greeting; }
    const std::string& GetPortraitPath1() const { return m_PortraitPath1; }
    const std::string& GetPortraitPath2() const { return m_PortraitPath2; }
    const std::vector<ShopItem>& GetItems() const { return m_Items; }

    ShopPurchaseResult Purchase(Player& player, int itemIndex) const;

private:
    std::string m_Title;
    std::string m_Greeting;
    std::string m_PortraitPath1;
    std::string m_PortraitPath2;
    std::vector<ShopItem> m_Items;
};

#endif
