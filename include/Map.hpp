#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "BackgroundImage.hpp" // 使用你現有的背景圖片類別

class Map {
public:
    Map();
    void Draw(); // 這是核心的雙層 for 迴圈所在

private:
    // 定義 11x11 的小陣列 (1 為牆壁，0 為地板)
    int m_MapData[11][11] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    BackgroundImage m_Wall;  // 牆壁物件
    BackgroundImage m_Floor; // 地板物件
    float m_TileSize = 32.0f; // 假設每個圖塊是 32x32 像素
};

#endif
