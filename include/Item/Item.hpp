#ifndef ITEM_HPP
#define ITEM_HPP

#include "App/BackgroundImage.hpp"

class Item : public BackgroundImage {
public:
    enum class ItemType {
        COIN = 30,
        RED_POTION = 31,
        BLUE_POTION = 32,
        RUBY = 33,
        SAPPHIRE = 34,
        SWORD_E = 35,
        MAGIC_KEY = 36,
        SHIELD_C = 37,
        JUMP_WING = 38,
        CROSS = 39,
        WIND_COMPASS = 40,
        SWORD_C = 41,
        SHIELD_D = 42,
        GEM_DIGGER = 43,
        BIG_WING = 44,
        HOLY_WATER = 45,
        SWORD_B = 46,
        SHIELD_A = 47,
        WING = 48,
        RED_VERI = 49,

    };

    Item(ItemType type);
    ItemType GetType() const { return m_Type; }

private:
    ItemType m_Type;
};

#endif