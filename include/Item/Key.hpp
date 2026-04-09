#ifndef KEY_HPP
#define KEY_HPP

#include "App/BackgroundImage.hpp"

class Key : public BackgroundImage {
public:
    enum class KeyType { YELLOW = 10, BLUE = 11, RED = 12 }; // 對應地圖編號

    Key(KeyType type);
    KeyType GetType() const { return m_Type; }

private:
    KeyType m_Type;
};

#endif