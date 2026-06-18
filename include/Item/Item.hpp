#ifndef ITEM_HPP
#define ITEM_HPP

#include "App/BackgroundImage.hpp"

class Item : public BackgroundImage {
public:
    enum class ItemType {
        YELLOWKEY = 10,
        BLUEKEY = 11,
        REDKEY = 12,
        COIN = 30,
        RED_POTION = 31,
        BLUE_POTION = 32,
        RUBY = 33,
        SAPPHIRE = 34,
        SWORD_E = 35,
        MAGIC_KEY = 36,
        SHIELD_C = 37,
        SMALL_WING = 38,
        CROSS = 39, // 給仙子後，全能力提升33%（也就是乘４／３後進位）
        WIND_COMPASS = 40, // 穿越樓層
        SWORD_C = 41,
        SHIELD_D = 42,
        GEM_DIGGER = 43,
        BIG_WING = 44,
        HOLY_WATER = 45,
        SWORD_B = 46,
        SHIELD_A = 47,
        RED_VERI = 48,
        GREEN_VERI = 49,
        GOD_KNIFE = 50,
    };

    Item(ItemType type);
    ItemType GetType() const { return m_Type; }

private:
    ItemType m_Type;
};

#endif