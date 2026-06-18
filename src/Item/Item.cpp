#include "Item/Item.hpp"

Item::Item(ItemType type) : m_Type(type), BackgroundImage("") {
    std::string path;
    switch (type) {
        case ItemType::YELLOWKEY:   path = RESOURCE_DIR "/Image/Item/yellow_key.bmp"; break;
        case ItemType::BLUEKEY:     path = RESOURCE_DIR "/Image/Item/blue_key.bmp"; break;
        case ItemType::REDKEY:      path = RESOURCE_DIR "/Image/Item/red_key.bmp"; break;
        case ItemType::COIN:        path = RESOURCE_DIR "/Image/Item/coin.bmp"; break;
        case ItemType::RED_POTION:  path = RESOURCE_DIR "/Image/Item/red_potion.bmp"; break;
        case ItemType::BLUE_POTION: path = RESOURCE_DIR "/Image/Item/blue_potion.bmp"; break;
        case ItemType::RUBY:        path = RESOURCE_DIR "/Image/Item/ruby.bmp"; break;
        case ItemType::SAPPHIRE:    path = RESOURCE_DIR "/Image/Item/sapphire.bmp"; break;
        case ItemType::SWORD_E:     path = RESOURCE_DIR "/Image/Item/sword_e.bmp"; break;
        case ItemType::MAGIC_KEY:   path = RESOURCE_DIR "/Image/Item/magic_key.bmp"; break;
        case ItemType::SHIELD_C:    path = RESOURCE_DIR "/Image/Item/shield_c.bmp"; break;
        case ItemType::SMALL_WING:   path = RESOURCE_DIR "/Image/Item/small_wing.bmp"; break;
        case ItemType::CROSS:       path = RESOURCE_DIR "/Image/Item/cross.png"; break;
        case ItemType::WIND_COMPASS:path = RESOURCE_DIR "/Image/Item/wind_compass.png"; break;
        case ItemType::SWORD_C:     path = RESOURCE_DIR "/Image/Item/sword_c.bmp"; break;
        case ItemType::SHIELD_D:    path = RESOURCE_DIR "/Image/Item/shield_d.bmp"; break;
        case ItemType::GEM_DIGGER:  path = RESOURCE_DIR "/Image/Item/gem_digger.bmp"; break;
        case ItemType::BIG_WING:    path = RESOURCE_DIR "/Image/Item/big_wing.png"; break;
        case ItemType::HOLY_WATER:  path = RESOURCE_DIR "/Image/Item/holy_water.bmp"; break;
        case ItemType::SWORD_B:     path = RESOURCE_DIR "/Image/Item/sword_b.bmp"; break;
        case ItemType::SHIELD_A:    path = RESOURCE_DIR "/Image/Item/shield_a.bmp"; break;
        case ItemType::RED_VERI:    path = RESOURCE_DIR "/Image/Item/red_veri.bmp"; break;
        case ItemType::GREEN_VERI:   path = RESOURCE_DIR "/Image/Item/green_veri.bmp"; break;
        case ItemType::GOD_KNIFE:   path = RESOURCE_DIR "/Image/Item/god_knife_sign.bmp"; break;
    }

    this->SetDrawable(std::make_shared<Util::Image>(path));
    this->SetScale({0.73f, 0.73f});
}