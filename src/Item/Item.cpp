#include "Item/Item.hpp"

Item::Item(ItemType type) : m_Type(type), BackgroundImage("") {
    std::string path;
    switch (type) {
        case ItemType::RED_POTION:  path = RESOURCE_DIR "/Image/Item/red_potion.bmp"; break;
        case ItemType::BLUE_POTION: path = RESOURCE_DIR "/Image/Item/blue_potion.bmp"; break;
        case ItemType::RUBY:        path = RESOURCE_DIR "/Image/Item/ruby.bmp"; break;
        case ItemType::SAPPHIRE:    path = RESOURCE_DIR "/Image/Item/sapphire.bmp"; break;
    }

    this->SetDrawable(std::make_shared<Util::Image>(path));
    this->SetScale({0.73f, 0.73f});
}